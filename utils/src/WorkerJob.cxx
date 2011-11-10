
#include "WorkJob.hxx"

enum {
    ERR_COMMAND =0,
    DATASEND_SPLIT,
    DATASEND_COMPLETE
} CommandType;

WorkJob::WorkJob(int sockid) {
    socket_ = sockid;
}

WorkJob::~WorkJob() {
}

bool
WorkJob::perform() {
	// implement the work to be done with the socket
	// in the virtual implementation of this function
}

CommandType
WorkJob::getCommand(BinaryData &binCommand) {
    if (binCommand.size_ != 4) {
        return ERR_COMMAND;
    }
    // FIXME : note, this assumes 'int' to be of 4 bytes
    int cmd = *((int *)binCommand.data_);
    return (CommandType) cmd;
}

int
WorkJob::getIntegerBinary(BinaryData &binInteger) {
    if (binInteger.size_ != 4) {
        return 0;
    }
    // FIXME : note, this assumes 'int' to be of 4 bytes
    return *((int *)binInteger.data_);
}


bool
WorkJob::recieve(vector<BinaryData *> &data) {
    // 1. get the command first
    // 2. get the size of data
    // 3. recieve the data
    // 4. if command is of split type repeat the steps
    BinaryData tempData;
    bool status = recieveAll(4, tempData);
    switch(getCommand(tempData)) {
        case ERR_COMMAND:
            return false;
        case DATASEND_SPLIT:
            {
                status = recieveAll(4, tempData);
                if (!status) return false;
                int length = getDataLength(tempData);
                status = recieveAll(length, tempData);
                if (!status) return false;
                data.push_back(new BinaryData(tempData));
                return this->recieve(data);
            }
        case DATASEND_COMPLETE:
            {
                status = recieveAll(4, tempData);
                if (!status) return false;
                int length = getDataLength(tempData);
                status = recieveAll(length, tempData);
                if (!status) return false;
                data.push_back(new BinaryData(tempData));
                return status;
            }
        default:
            break;
    }
    return false;
}

bool
WorkJob::send(int length, char* data) {
    return sendAll(length +8, this->streamLineBinData(length, data));
}

bool
WorkJob::send(string &data) {
    return sendAll(data.length() +8, this->streamLineBinData(data.length(), data.c_str()));
}

bool
WorkJob::send(BinaryData *binData) {
    return sendAll(binData->size_ +8, this->streamLineBinData(binData));
}

bool
WorkJob::send(vector<BinaryData*> &binDataVector) {
    vector<BinaryData *>::iterator iter;
    BinaryData *lastElem = binDataVector.back();
    binDataVector.pop_back();
    int total =0;
    vector<char *> charVector;
    for (iter = binDataVector.begin(); iter != binDataVector.end(); iter++) {
        BinaryData *binData = (BinaryData *) (*iter);
        total += binData.size_ +8;
    }
    total += lastElem.size_ +8;

    char *buf = new char[total];
    char *localBuf = buf;
    for (iter = binDataVector.begin(); iter != binDataVector.end(); iter++) {
        BinaryData *binData = (BinaryData *) (*iter);
        char *stream = this->streamLineBinData(binData, true);
        memcpy(localBuf, stream);
        localBuf += binData.size_ +8;
        delete []stream;
    }
    char *stream = this->streamLineBinData(lastElem);
    memcpy(localBuf, stream);
    delete []stream;

    this->send(total, buf);
    delete []buf;
}

bool
WorkJob::sendAll(int size, const char* data) {
    int total = 0;        // how many bytes we've sent
    int bytesleft = size; // how many we have left to send
    int n;

    while(total < size) {
        n = send(socket_, data+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    return (n!=-1); // return -1 on failure, 0 on success
}

bool
WorkJob::recieveAll(int size, BinaryData &data) {
    char buf[size];
    int bytes_recieved = recv(socket_, buf, size, MSG_WAITALL);
    if (bytes_recieved == -1 || bytes_recieved == 0)  {
        return false;
    }
    data.assign(size, buf);
    return true;
}

char*
WorkJob::streamLineBinData(BinaryData* data, bool split = false) {
    return this->streamLineBinData(data->size_, data->data_);
}

char*
WorkJob::streamLineBinData(int len, const char* data, bool split = false) {
    char *buf = new char[len +8];
    *((int *)buf) = split?DATASEND_SPLIT:DATASEND_COMPLETE;
    *((int *)(buf +4)) = len;
    memcpy(buf +8, data);
    return buf;
}

//	public:
//		int	socket_;
