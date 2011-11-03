
#include "BaseThread.hxx"
#include <pthread.h>
#include <queue>

using namespace std;

BaseThread::BaseThread(void *arg =0) {
    _arg = arg;
}

BaseThread::~BaseThread() {
}

void *
LaunchFunction(void *arg) {
	BaseThread *obj = (BaseThread *) arg;
	obj->run();
}

void
BaseThread::start() {
    pthread_t *th = new pthread_t;
    pthread_attr_t custom_attr;
    pthread_attr_init(&custom_attr);
    pthread_create(th, &custom_attr, &LaunchFunction, (void *) this);
	_threadIdQueue.push(th);
}

