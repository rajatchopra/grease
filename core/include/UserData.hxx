
#ifndef __USER_DATA_HXX__
#define __USER_DATA_HXX__

#include <map>

using namespace std;

class UserData : public BaseObject {
    public:
        UserData(const char *key, const int dataSize, const char *data);
        ~UserData();
    public:
        void    writeToFile(FILE *fp);
    private:
        char*   _key;
        int     _size;
        char*   _data;
};

#endif

