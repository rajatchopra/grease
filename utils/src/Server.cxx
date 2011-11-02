
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
		this->_clientQueue.push_back(sd);
		
	}

	close(sock);

	return 0;
}

void
Server::handleClients() {
	cout << "got connection " << endl; //from " << pin.sin_addr << endl;
	close(sd);
}

