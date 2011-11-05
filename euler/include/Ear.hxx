
#ifndef __EAR_HXX__
#define __EAR_HXX__

class Ear : public Server {
};

class EarWork : public WorkJob {
    public:
        EarWork();
        ~EarWork();
    public:
        virtual void perform();
};

#endif

