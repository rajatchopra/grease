
#include "BinaryData.hxx"

BinaryData::BinaryData(int size, const char* data) {
    if (data && size) {
        data_ = new char[size];
        memcpy(data_, data, size);
        size_ = size;
    } else {
        size_ =0;
        data_ =0;
    }
}

BinaryData::~BinaryData() {
    if (data_) delete []data_;
    size_ =0;
    data_ =0;
}

BinaryData::BinaryData(const BinaryData &copyfrom) {
    this->size_ = copyfrom.size_;
    if (this->data_) {
        delete [] this->data_;
    }
    this->data_ = new char[this->size_];
    memcpy(this->data_, copyfrom.data_, this->size_);
}

void
BinaryData::merge(BinaryData &addend) {
    char *temp = new char[this->size_ + addend.size_];
    if (this->data_) {
        mempcy(temp, this->data_, this->size_);
        delete []this->data_;
    }
    memcpy(temp + this->size_, addend.data_, addend.size);
    this->size_ += addend.size_;
    this->data_ = temp;
}

BinaryData&
BinaryData::operator+=(const BinaryData &addend) {
    this->merge(addend);
    return (*this);
}

BinaryData&
BinaryData::operator=(const BinaryData &addend) {
    if (data_) delete []data_;
    size_ =0;
    data_ =0;
    this->merge(addend);
    return (*this);
}

void
BinaryData::assign(int size, const char*data) {
    if (data_) delete []data_;
    if (data && size) {
        data_ = new char[size];
        memcpy(data_, data, size);
        size_ = size;
    } else {
        size_ =0;
        data_ =0;
    }
}

//    public:
//        int     size_;
//        char*   data_;
