
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
    return sendAll(length, data);
}

bool
WorkJob::send(BinaryData *binData) {
    return sendAll(binData->size_, binData->data_);
}

bool
WorkJob::sendAll(int size, char* data) {
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

//	public:
//		int	socket_;
