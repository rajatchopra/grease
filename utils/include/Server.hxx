
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
		queue<int>	_clientQueue;
};

#endif

