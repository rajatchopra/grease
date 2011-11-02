
#include <String>
#include <stdio.h>
#include <stdlib.h>
#include "UserInterface.hxx"


UserInterface::UserInterface(const char *clusterName, const char *configDir) {
    _configPath = String(configDir);
    _clusterName = String(clusterName);
    _recylcer.start()
}

UserInterface::~UserInterface() {
    if (_fileName) delete _fileName;
    _fileName =0;
}

void 
UserInterface::write(const char* bucket, const char *key, int size, char *data) {
    this->lock();
    UserTable *table = this->getBucketTable(bucket);
    UserData *newData = new UserData(key, size, data);
    table->write(newData);
    this->unlock();
}

char*
UserInterface::read(const char* bucket, const char* key, int &length) {
    this->lock();
    UserTable *table = this->getBucketTable(bucket);
    UserData *fetchedData = table->read(key);
    this->unlock();
    length = fetchedData->getSize();
    char *returnData = fetchedData->getData();
    return returnData;
}

String
UserInterface::getFileName(const char*key) {
    String fileName;
    fileName += _configPath + "/" + _clusterName + "/" + String(key) + String(".grease");
    return fileName.c_str();
}

UserTable*
UserInterface::getBucketTable(const char *bucket) {
    if (_bucketMap.find(String(bucket))==_bucketMap.end()) {
        const char *fileName = this->getFileName(bucket).c_str();
        UserTable *newTable = UserTable::readFromFile(fileName);
        if (!newTable) newTable = new UserTable(bucket);
        _bucketMap.insert(String(bucket), newTable);
    }
    return _bucketMap[String(bucket)];
}

//private:
//    map<String, UserTable*>     _bucketMap;
//    Recycler    _recylcer;
//    String       _configPath;
//    String       _clusterName;
