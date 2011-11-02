
#ifndef __BASE_OBJECT_HXX__
#define __BASE_OBJECT_HXX__

#include <pthread.h>

class BaseObject {
    public:
        BaseObject(int size, char *data);
        ~BaseObject();
    public:
        bool    lock();
        bool    unlock();
    private:
        pthread_mutex_t*    _mutex;
};

#endif

