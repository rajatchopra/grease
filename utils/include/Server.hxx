
#ifndef __SERVER_HXX__
#define __SERVER_HXX__

#include <queue>

using namespace std;

class Server {
	public:
		Server();
		~Server();

	public:
		void serve();

	private:
		void run(void *arg);
		void handleClients();

	private:
		fd_set				_clientSet;
		queue<int>			_clientQueue;
		pthread_mutex_t*	_clientQueueMutex;
		int					_max_sd;
};

#endif

