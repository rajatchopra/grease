

#include "Worker.hxx"

WorkerThread::WorkerThread() {
}

WorkerThread::~WorkerThread() {
}

void	
WorkerThread::run(void *arg) {
}

void	
WorkerThread::addJob(WorkJob *job) {
}

void	
WorkerThread::trigger() {
}

bool	
WorkerThread::ready() {
}


//	private:
//		queue<WorkJob *>	_workQueue;
//		bool				_busy;
//		pthread_mutex_t*	_trigger;
//		pthread_mutex_t*	_queMutex;
