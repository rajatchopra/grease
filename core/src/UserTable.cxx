
#include "UserTable.hxx"
#include <stdlib.h>
#include <stdio.h>

UserTable::UserTable(const char*bucket) {
    this->key = new char[strlen(bucket) +1];
    strcpy(this->key, bucket);
    _versionNumber =0;
    _userDataHash =0;
    _inMemory = true;
}

UserTable::~UserTable() {
    // lazy to even copy paste
    this->renewUserDataHash();
    delete _userDataHash;
}

void
UserTable::write(UserData* uData) {
    this->lock();
    if (!_userDataHash) this->renewUserDataHash();
    _userDataHash[uData->getKey()] = uData;
    _versionNumber++;
    this->unlock();
}

UserData*   
UserTable::read(const char* key) {
    this->lock();
    if (!_userDataHash) return 0;
    UserData *returnData = _userDataHash[String(key)];
    this->unlock();
    return returnData;
}

void
UserTable::writeToFile(FILE *fp) {
    return;
}

static UserTable*
UserTable::readFromFile(const char *filename) {
    FILE *fp = fopen(fileName, "r");
    if (fp) fclose(fp);
    return 0;
}

void
UserTable::renewUserDataHash() {
    if (_userDataHash) {
        map<String, UserData *>::iterator iter;
        for (iter=_userDataHash.begin(); iter!=_userDataHash.end(); iter++) {
            UserData *data = (*iter).second;
            delete data;
        }
        delete _userDataHash;
    }
    _userDataHash = new map<String, UserData*>;
    _versionNumber =0;
}

//private:
//map<String, UserData *>*    _userDataHash;
//int             _versionNumber;
//bool            _inMemory;
