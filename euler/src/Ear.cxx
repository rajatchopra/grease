
#include "Ear.hxx"
#include "GcEnum.hxx"

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

GcCommandType
EarWork::recieveCommand() {
    vector<BinaryData*> data;
    bool status = this->recieve(data);
    if (!status) return GC_ERR_COMMAND;
    if (data.length() != 1) return GC_ERR_COMMAND;

    BinaryData *cmdData = data[0];
    GcCommandType cmd = (GcCommandType) this->getIntegerBinary(cmdData);
    delete cmdData;
    return cmd;
}

bool
EarWork::perform() {
    // 1. Get membership data
    // 2. Get data Versions
    // 3. Seek new data

    LocalDB *db = LocalDB::getLocalDB();
    while (true) {
        char **argv =0;
        int argc =0;
        switch(this->recieveCommand()) {
            case GC_MEMBERSHIP:
                {
                    MemberTable *table = db->getMemberTable();
                    string dump = table->serialize();
                    this->send(dump, dump.length());
                    break;
                }
            case GC_DATA_VERSIONS:
                {
                    map<string, string> vmap = db->getVersionMap();
                    BinaryData *dump = Utils::serializeMap(vmap);
                    this->send(dump);
                    delete dump;
                    break;
                }
            case GC_DATA_TABLES:
                {
                    // get which table
                    vector<BinaryData*> data;
                    bool status = this->recieve(data);
                    if (!status) return;
                    vector<BinaryData *>::iterator iter;
                    map<string, BinaryData *> sendData;
                    for (iter = data.begin(); iter != data.end(); iter++) {
                        BinaryData *bucket = (*iter);
                        char *bucketName = bucket->toStr();
                        delete bucket;

                        BinaryData *bson = db->bucketSerialize(bucketName);
                        sendData[string(bucketName)] = bson;
                        delete [] bson;
                        delete [] bucketName;
                    }
                    BinaryData *dump = Utils::serializeMap(sendData);
                    this->send(dump);
                    delete dump;
                    // we are done with sending tables, this socket can
                    // be sent for monitoring in case there are further requests
                    // but for now we will not block this worker thread
                    return true;
                }
            case GC_ERR_COMMAND:
                return false;
            case GC_CONN_CLOSED_ERR:
                return false;
            case GC_TIMEOUT_ERR:
                return true;
            default:
                return false;
                break;
        }
    }
    // flow never reaches here
    return;
}

