
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <pthread.h>

#include "Server.hxx"

using namespace std;

Server::Server(bool handlePersistence =false, bool runInBackground =false, int port = 5120) {
	if (handlePersistence) {
		_persistentClientHandler = new PersistentClientHandler(this);
	} else {
		_persistentClientHandler =0;
	}
	_background = runInBackground;
	_port = port;
}

Server::~Server() {
	if (_persistentClientHandler) delete _persistentClientHandler;
}

void
Server::serve() {
	this->prepareWorkers();
	if (_persistentClientHandler) _persistentClientHandler->start();
	_background?this->start():this->run();
}

void
Server::prepareWorkers() {
	int i =0;
	for (i =0; i<10; i++) {
		_workerThreads.push_back(this->getNewWorkerThread());
	}
}

void
Server::run() {
	struct sockaddr_in sin;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0) {
		perror("Socket creation");
	}

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(_port);

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
	 	this->pushToWorkerThread(sd);
	}

	close(sock);

	return 0;
}


WorkerThread*
Server::getNewWorkerThread() {
	WorkerThread *worker = new WorkerThread(this);
	worker->start();
	return worker;
}

WorkJob*
Server::getNewWorkJob(int sock) {
	WorkJob *job = new WorkJob(sock);
	return job;
}

void
Server::closeJob(WorkJob *job, bool status) {
	// worker thread has indicated that its done with this socket
	if (_persistentClientHandler && status) {
		_persistentClientHandler->addClient(job);
	}
	else {
		delete job;
	}
}

void
Server::pushToWorkerThread(int sock) {
	WorkJob *job = this->getNewWorkJob(sock);
	this->pushToWorkerThread(job);
}

void
Server::pushToWorkerThread(WorkJob *job) {
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


////////////////////////////////////////////////////////////////
// PersistentClientHandler Implementation 
////////////////////////////////////////////////////////////////


PersistentClientHandler::PersistentClientHandler(Server *server) {
	_server = server;
	_clientQueueMutex = new pthread_mutex_t;
	pthread_mutex_init(_clientQueueMutex);
}

PersistentClientHandler::~PersistentClientHandler() {
	if (_clientQueueMutex) delete _clientQueueMutex;
}

void
PersistentClientHandler::run() {
	// select and loop through the _clientSet of all sockets
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tc_usec = 0;
	while (1) {
		if (_max_sd < sd) _max_sd = sd;
		
		pthread_mutex_lock(_clientQueueMutex);
		// make a copy of the queue and check which socket is ready
		queue<WorkJob *>::iterator iter;
		FD_CLR(_clientSet);
		for (iter=_clientQueue.begin() ; iter!=_clientQueue.end() ; iter++) {
			FD_SET(_clientSet, (*iter)->socket_);
		}
		queue<WorkJob *> clients = _clientQueue;
		pthread_mutex_unlock(_clientQueueMutex);

		int n = select(_max_sd, _clientSet, 0, 0, &tv);
		switch (n) {
			case -1:
				perror("Select FD");
				break;
			case 0:
				break;
			default: 
				{
					// deal with this socket through a worker thread
					queue<WorkJob *>::iterator iter;
					for (iter = clients.begin(); iter != clients.end(); iter++) {
						WorkJob *job = (*iter);
						int sock = job->socket_;
						if (FD_ISSET(sock, _clientSet)) _server->pushToWorkerThread(job);
					}
				}
				break;
		}
	}
}

void
PersistentClientHandler::addClient(WorkJob *job) {
	pthread_mutex_lock(_clientQueueMutex);
	_clientQueue.push(job);
	pthread_mutex_unlock(_clientQueueMutex);
}

