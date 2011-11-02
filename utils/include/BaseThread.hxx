
#ifndef __BASE_THREAD_HXX__
#define __BASE_THREAD_HXX__

class BaseThread {
    public:
        BaseThread(void *arg);
        ~BaseThread();
    public:
        void start();
        void run(void *arg) =0;
    private:
        void*   _arg;
};

#endif

