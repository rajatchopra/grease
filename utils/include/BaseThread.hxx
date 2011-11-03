
#ifndef __BASE_THREAD_HXX__
#define __BASE_THREAD_HXX__

#include <pthread.h>
#include <queue>

using namespace std;

class BaseThread {
    public:
        BaseThread(void *arg);
        ~BaseThread();
    public:
        void start();
        virtual void run() =0;
    private:
        void*   _arg;
		queue<pthread_t *>	_threadIdQueue;
};

#endif

