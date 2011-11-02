
#ifndef __WORKER_HXX__
#define __WORKER_HXX__

#include "WorkJob.hxx"
#include "BaseThread.hxx"

class WorkerThread : public BaseThread {
	public:
		WorkerThread(pthread_mutex_t *trigger_mutex, queue<WorkJob> *workQueue, void *arg);
		~WorkerThread();

	public:
		void	run(void *arg);
		bool	ready();
	private:
		pthread_mutex_t*	_trigger;
		bool				_busy;
};

#endif

