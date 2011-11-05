
#include "Ear.hxx"

Ear::Ear(int portNumber) : Server(false, false, portNumber) {
}

Ear::~Ear() {
}

WorkJob*
Ear::getNewWorkJob(int sock) {
    return new EarWork(sock);
}

EarWork::EarWork(int sock) : WorkJob(sock) {
}

EarWork::~EarWork() {
}

void
EarWork::perform() {
    // 1. Get membership data
    // 2. Get data Versions
    // 3. Seek new data

    LocalDB *db = LocalDB::getLocalDB();
    while (true) {
        char **argv =0;
        int argc =0;
        switch(this->recieveCommand(argc, argv)) {
            case GC_MEMBERSHIP:
                {
                    MemberTable *table = db->getMemberTable();
                    string dump = table->to_json();
                    this->sendData(dump, strlen(dump));
                    break;
                }
            case GC_DATA_VERSIONS:
                {
                    db->getVersionDump();
                }
            case GC_DATA_TABLES:
                {
                    int i =0;
                    for (i =0; i <argc; i++) {
                        char *bucketName = argv[i];
                        char *bjson = db->to_json(bucketName);
                        db->update(bucketName, bjson);
                        delete []bucketName;
                    }
                }
            default:
                return;
                break;
        }
    }
    // flow never reaches here
    return;
}

