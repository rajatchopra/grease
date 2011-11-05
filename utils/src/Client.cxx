
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/time.h>

#include "Client.hxx"

Client::Client(const char *hostname, int portNo) {
    _hostName =0;
    if (hostname) {
        _hostName = new char[strlen(hostname) + 1];
        strcpy(_hostName, hostname);
        _port = portNo;
    }
}

Client::~Client() {
    if (_hostName) {
        delete _hostName;
    }
}

bool
Client::connect(WorkJob *job) {
    if (!_hostName) return false;

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return false
    }
    server = gethostbyname(_hostName);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return false;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(_portNo);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        return false;
    }

    job->socket_ = sockfd;
    job->perform();
    close(sockfd);
    return true;
}

