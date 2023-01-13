#ifndef MESSAGE
#define MESSAGE

#define BUFFER_SIZE (1024)

enum MessageTypes {
    EXIT = 0,
    CONNECT = 1,
    CONNACK = 2,
    LIST = 3,
    LISTREPLY = 4,
    INFO = 5,
    INFOREPLY = 6,
    SEND = 7,
    SENDREPLY = 8,
    RECEIVE = 9,
    RECEIVEREPLY = 10,
};

class Message {
    private:
        MessageTypes type;
        short id;
        short length;
        char message[BUFFER_SIZE] = { 0 }, message_packet[BUFFER_SIZE] = { 0 };

        short getTypeAndId(int type, short id);
    
    public:
        Message(const char *message);
        Message(MessageTypes type, short id, short length, const char *message);
        Message(MessageTypes type, short id, short length, const char message);
        MessageTypes getType() { return type; }
        short getId() { return id; }
        char* getMessage() { return message; }
        char* getMessagePacket() { return message_packet; }
        short getLength() { return length; }
};

#endif