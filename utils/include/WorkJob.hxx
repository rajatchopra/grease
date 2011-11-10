
#ifndef __WORK_JOB_HXX__
#define __WORK_JOB_HXX__

class WorkJob {
	public:
		WorkJob(int sockid);
		~WorkJob();
	public:
        void            recieve(int &argc, char ** &data);
        bool            send(int argc, char** data);
        bool            send(int length, char* size);
        bool            send(string data);
        bool            send(BinaryData *binData);
		virtual bool    perform();
    private:
        bool sendAll(int size, const char* data);
        bool recieveAll(int size, BinaryData &data);
        BinaryData      recieveAll();
	public:
		int	socket_;
};

#endif

