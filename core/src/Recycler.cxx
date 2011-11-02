
#include "Recycler.hxx"
#include "UserInterface.hxx"

#define MEM_LIMIT 32

Recycler::Recycler (UserInterface *db) : BaseThread(NULL) {
    _db = db;
}

Recycler::~Recycler () {
    _db =0;
}

void
Recycler::run(void *arg) {
    // this function runs in a separate thread
    // it should wake up periodically and check the memory usage
    // and if a trim is needed, then it should flush
    // some random table out of the db
    while (true) {
        sleep(10.0);
        int size = _db->calculateSize();
        if (size/(1024*1024) > MEM_LIMIT) {
            // flush something out
        }
    }
}

//    private:
//        UserInterface*  _db;
