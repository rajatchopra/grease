
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#include "server.hxx"

using namespace std;

Server::Server() {
}

Server::~Server() {
}

void
Server::serve() {
	this->handleClients();
	this->start();
}

void
Server::run(void *arg) {
	struct sockaddr_in sin;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0) {
		perror("Socket creation");
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(5120);

	if (bind(sock, (struct sockaddr *)  &sin, sizeof(sin)) < 0) {
		perror("Socket bind");
	}

	if (listen(sock, 10) < 0) {
		perror("Socket listen");
	}

	// now start the server accept loop
	struct sockaddr_in pin;
	socklen_t addr_len = sizeof(pin);
	while (true) {
	    cout << "Waiting for a connection...." << endl;
	    int sd = accept(sock, (struct sockaddr *) &pin, &addr_len);
	    if (sd < 0) {
			perror("Socket accept");
		}
		pthread_mutex_lock(_clientQueueMutex);
		_clientQueue.push_back(sd);
		pthread_mutex_unlock(_clientQueueMutex);
	}

	close(sock);

	return 0;
}

void
Server::handleClients() {
	// prepare worker threads
	
	// select and loop through the _clientSet of all sockets
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tc_usec = 0;
	while (1) {
		if (_max_sd < sd) _max_sd = sd;
		
		pthread_mutex_lock(_clientQueueMutex);
		// make a copy of the queue and check which socket is ready
		queue<int> clients = _clientQueueMutex;
		pthread_mutex_unlock(_clientQueueMutex);

		int n = select(_max_sd, wk_set, 0, 0, &tv);
		switch (n) {
			case -1:
				perror("Select FD");
				break;
			case 0:
				break;
			default: 
				{
					// deal with this socket through a worker thread
					queue<int>::iterator iter;
					for (iter = clients.begin(); iter != clients.end(); iter++) {
						sock = (*iter);
						if (FD_ISSET(sock, &wk_set)) this->pushToWorkerThread(sock);
					}
				}
				break;
		}
	}
}

WorkerThread*
Server::getNewWorkerThread() {
	WorkerThread *worker = new WorkerThread();
	worker->start();
	return worker;
}

WorkJob*
Server::getNewWorkJob(int sock) {
	WorkJob *job = new WorkJob(sock);
	return job;
}

void
Server::pushToWorkerThread(int sock) {
	WorkJob *job = this->getNewWorkJob(sock);
	vector<WorkerThread *>::iterator iter;
	for (iter = _workerThreads.begin(); iter != _workerThreads.end(); iter++) {
		WorkerThread *worker = (*iter);
		if (worker->ready()) {
			worker->addJob(job);
			worker->trigger();
			return;
		}
	}
	// if the flow reaches here, either we add new worker threads, 
	// or just queue up on some random existing worker thread
	int max = _workerThreads.length; 
	int rand = rand()%max;
	WorkerThread *worker = _workerThreads[rand];
	if (worker) {
		worker->addJob(job);
		worker->trigger();
	}
	else {
		// scream error
	}
	return;
}

