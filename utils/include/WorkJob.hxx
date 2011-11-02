
#ifndef __WORK_JOB_HXX__
#define __WORK_JOB_HXX__

class WorkJob {
	public:
		WorkJob(int sockid);
		~WorkJob();
	private:
		int	_socket;
};

#endif

