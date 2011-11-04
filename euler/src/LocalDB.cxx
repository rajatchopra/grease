
#include "LocalDB.hxx"
#include "UserInterface.hxx"

LocalDB*    LocalDB::_instance =0;
char*       LocalDB::_soModuleFile =0;
char*       LocalDB::_soModuleName =0;

LocalDB::LocalDB() {
    _localUserInterface =0;
    if (_soModuleFile) {
        // dlopen the file
    } else {
        _localUserInterface = new UserInterface();
    }
}

LocalDB *
LocalDB::getLocalDB() {
    if (this->_instance) return this->_instance;
    return new LocalDB();
}

LocalDB::~LocalDB() {
}

void
LocalDB::write(const char* bucket, const char *key, int size, const char *data) {
    if (_localUserInterface) {
        _localUserInterface(bucket, key, size, data);
    }
}

char*
LocalDB::read(const char *bucket, const char *key, int &length) {
    if (_localUserInterface) {
        return _localUserInterface(bucket, key, size, data);
    }
}

