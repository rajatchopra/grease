
#ifndef __BINARY_DATA_HXX__
#define __BINARY_DATA_HXX__

class BinaryData {
    public:
        BinaryData(int size =0, const char* data =0);
        BinaryData(const BinaryData &copyfrom);
        ~BinaryData();
    public:
        void    merge(BinaryData &addend);
        BinaryData& operator += (const BinaryData &first);
        char*   toStr();
    public:
        int     size_;
        char*   data_;
};

#endif

