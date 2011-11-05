
#ifndef __WORK_JOB_HXX__
#define __WORK_JOB_HXX__

class WorkJob {
	public:
		WorkJob(int sockid);
		~WorkJob();
	public:
		virtual bool perform();
	public:
		int	socket_;
};

#endif

