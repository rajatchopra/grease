
#include "Mouth.hxx"
#include "GcEnum.hxx"

Mouth::Mouth(int interval) {
    _interval = interval;
}

Mouth::~Mouth() {
}

void
Mouth::run() {
    // sleep for '_interval' seconds
    // then reach out for the next guy in the membership table
    // and ask for sync up
}


//    private:
//        Client* _client;


////////////////////////////////////////
// MouthWork class implementation
///////////////////////////////////////

MouthWork::MouthWork() {
}

MouthWork::~MouthWork() {
}

void
MouthWork::sendCommand(GcCommandType com) {
    int data = (int) com;
    BinaryData *binData = new BinaryData(4, &data);
    this->send(binData);
    delete binData;
}

bool
MouthWork::perform() {
    // 1. ask for membership table
    // 2. ask for db table versions
    // 3. ask for specific tables

    // step 1
    sendCommand(GC_MEMBERSHIP);
    vector<BinaryData *> data;
    bool status = recieve(data);
    if (!status) return false;

    BinaryData *memberTableDump = data[0];
    MemberTable *table = MemberTable::deSerialize(memberTableDump);
    db->updateMembership(table);
    delete memberTableDump;
    data.clear();
    
    // step 2
    sendCommand(GC_DATA_VERSIONS);
    status = recieve(data);
    if (!status) return false;

    // process step 2 to prepare data for step 3
    BinaryData* versionMap = data[0];
    map<string, BinaryData *> vmap = Utils::deSerializeMap(versionMap);
    map<string, BinaryData *>::iterator iter;
    vector<string> outOfDateBuckets;
    for (iter = vmap.begin(); iter != vmap.end(); iter++) {
        string bucketName = (*iter).first;
        char *vNumber = ((*iter).second)->toStr();
        char *currentVersion = db->getBucketVersion(bucketName.c_str());
        if (atoi(vNumber) > atoi(currentVersion)) {
            outOfDateBuckets.push_back(bucketName);
        }
        delete []vNumber;
        delete (*iter).second;
    }
    delete versionMap;
    data.clear();

    // step 3
    sendCommand(GC_DATA_TABLES);
    send(outOfDateBuckets);

    status = recieve(data);
    if (!status) return false;

    BinaryData *allOutOfDateTables = data[0];
    map<string, BinaryData*> dmap = Utils::deSerializeMap(allOutOfDateTables);
    delete allOutOfDateTables;
    data.clear();

    for (iter = dmap.begin(); iter != dmap.end(); iter++) {
        string bucketName = (*iter).first;
        BinaryData *table = (*iter).second;
        db->updateTable(bucketName, table);
    }

    // since we do not want this session to be preserved we send false
    // alternately in the future we could re-use this socket for the sporadic sync-up
    return false;
}

