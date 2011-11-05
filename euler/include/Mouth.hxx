
#ifndef __MOUTH_HXX__
#define __MOUTH_HXX__

#include "BaseThread.hxx"
#include "Client.hxx"

class Mouth : public BaseThread {
    public:
        Mouth();
        ~Mouth();
    public:
        virtual void run();
    private:
        Client* _client;
};

class MouthWork : public WorkJob {
    public:
        MouthWork();
        ~MouthWork();
    public:
        virtual void perform();
}

#endif

