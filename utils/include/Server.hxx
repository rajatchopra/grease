
#ifndef __SERVER_HXX__
#define __SERVER_HXX__

#include <queue>

using namespace std;

class Server : public BaseThread {
	public:
		Server(bool handlePersistentClients =false, bool runInBackground =false, int port =5120);
		~Server();

	public:
		void serve();
		virtual WorkJob* getNewWorkJob(int sock);
		virtual WorkerThread* getNewWorkerThread();
		void closeJob(WorkJob *job, bool status);

	private:
		virtual void run();
		void pushToWorkerThread(int sock);
		void pushToWorkerThread(WorkJob *job);

	private:
		int							_port;
		vector<WorkerThread *>		_workerThreads;
		PersistentClientHandler*	_persistentClientHandler;
};

class PersistentClientHandler : public BaseThread {
	public:
		PersistentClientHandler(Server *);
		~PersistentClientHandler();
	public:
		void run();
		void addClient(WorkJob *job);
	private:
		int						_max_sd;
		fd_set					_clientSet;
		Server*					_server;
		queue<WorkJob *>		_clientQueue;
		pthread_mutex_t*		_clientQueueMutex;
};

#endif

