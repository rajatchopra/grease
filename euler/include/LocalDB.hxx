
#ifndef __LOCAL_DB_HXX__
#define __LOCAL_DB_HXX__

#include "UserInterface.hxx"

class LocalDB {
    protected:
        LocalDB(const char *so_file_path =0);
    public:
        static LocalDB *   getLocalDB();
        ~LocalDB();
    public:
        void    write(const char* bucket, const char *key, int size, const char *data);
        char*   read(const char *bucket, const char *key, int &length);
    private:
        static LocalDB*     _instance;
        static char*        _soModuleFile;
        static char*        _soModuleName;
        UserInterface*      _localUserInterface;
};

#endif

