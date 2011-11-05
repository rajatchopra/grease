
#ifndef __CLIENT_HXX__
#define __CLIENT_HXX__

class Client {
    public:
        Client(const char *hostname);
        ~Client();
    public:
        bool    connect(WorkJob *job);
};

#endif

