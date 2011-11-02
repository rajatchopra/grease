
#include "BaseThread.hxx"
#include <pthread.h>

BaseThread::BaseThread(void *arg =0) {
    _arg = arg;
}

BaseThread::~BaseThread() {
}

void
BaseThread::start() {
    pthread_t th;
    pthread_attr_t custom_attr;
    pthread_attr_init(&custom_attr);
    pthread_create(&th, &custom_attr, this->run, _arg);
}

