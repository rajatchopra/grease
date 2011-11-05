

#include "Worker.hxx"

WorkerThread::WorkerThread(Server *server) {
	_server = server;

	_queMutex = new pthread_mutex_t;
	pthread_mutex_init(_queMutex);
	_trigger = new pthread_mutex_t;
	pthread_mutex_init(_trigger);
	
	// lock it up
	pthread_mutex_lock(_trigger);

	_busy = false;
}

WorkerThread::~WorkerThread() {
	delete _queMutex;
	delete _trigger;
}

void	
WorkerThread::run(void *arg) {
	while (true) {
		_busy = false;
		pthread_mutex_lock(_trigger);
		_busy = true;
		// drain the queue and call perform on the job
		pthread_mutex_lock(_queMutex);
		queue<WorkJob *> localqueue = _workQueue;
		pthread_mutex_unlock(_queMutex);
		while (!localqueue.empty()) {
			WorkJob *job = localqueue.pop();
			if (job) {
				bool status = job->perform(); 
				_server->closeJob(job, status);
			}
		}
	}
}

void	
WorkerThread::addJob(WorkJob *job) {
	pthread_mutex_lock(_queMutex);
	_workQueue.push(job);
	pthread_mutex_unlock(_queMutex);
}

void	
WorkerThread::trigger() {
	pthread_mutex_unlock(_trigger);
}

bool	
WorkerThread::ready() {
	return !_busy;
}


//	private:
//		queue<WorkJob *>	_workQueue;
//		bool				_busy;
//		pthread_mutex_t*	_trigger;
//		pthread_mutex_t*	_queMutex;
