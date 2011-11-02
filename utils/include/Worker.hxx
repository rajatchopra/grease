
#ifndef __WORKER_HXX__
#define __WORKER_HXX__

#include "WorkJob.hxx"
#include "BaseThread.hxx"

class WorkerThread : public BaseThread {
	public:
		WorkerThread(void *arg);
		~WorkerThread();

	public:
		void	run(void *arg);
		void	addJob(WorkJob *job);
		void	trigger();
		bool	ready();
	private:
		queue<WorkJob *>	_workQueue;
		bool				_busy;
		pthread_mutex_t*	_trigger;
		pthread_mutex_t*	_queMutex;
};

#endif

