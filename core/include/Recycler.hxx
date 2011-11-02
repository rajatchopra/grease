
#ifndef __RECYCLER_HXX__
#define __RECYCLER_HXX__

#include <map>
#include "BaseThread.hxx"

using namespace std;

class UserInterface;

class Recycler : public BaseThread {
    public:
        Recycler (UserInterface *db);
        ~Recycler ();
    public:
        void run();
    private:
        UserInterface*  _db;
};

#endif

