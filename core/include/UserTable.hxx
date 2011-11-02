
#ifndef __USER_TABLE_HXX__
#define __USER_TABLE_HXX__

#include <map>
#include <String>

using namespace std;

class UserTable : public UserData {
    public:
        UserTable(const char *bucketName);
        ~UserTable();
    public:
        void        write(UserData* uData);
        UserData*   read(const char* key);
    public:
        void        writeToFile(FILE *fp);
        static UserTable*  readFromFile(const char *filename);
    private:
        void        renewUserDataHash();
    private:
        map<String, UserData *>*    _userDataHash;
        int             _versionNumber;
        bool            _inMemory;
};

#endif

