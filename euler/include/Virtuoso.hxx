
#ifndef __VIRTUOSO_HXX__
#define __VIRTUOSO_HXX__

class Virtuoso : public BaseThread {
    // container that manages consistency of data across the cluster
    private:
        Ear*    _ear;
        Mouth*  _mouth;
};

#endif

