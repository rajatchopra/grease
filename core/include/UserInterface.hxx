
#ifndef __USER_INTERFACE_HXX__
#define __USER_INTERFACE_HXX__

#include <map>
#include <String>
#include "Recycler.hxx"

using namespace std;

class UserInterface : public BaseObject {
    public:
        UserInterface();
        ~UserInterface();
    public:
        void        write(const char* bucket, const char *key, int size, char *data);
        char*       read(const char* bucket, const char* key, int &length);
    private:
        char*       getFileName(const char*key);
        UserTable*  getBucketTable(const char *bucketName);
    private:
        map<String, UserTable*>     _bucketMap;
        Recycler    _recylcer;
        String       _configPath;
        String       _clusterName;

};

#endif

