#ifndef MESSAGE
#define MESSAGE

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
    public:
        MessageTypes type;
        short id;
        short length;
        char *message, *message_packet;

        unsigned char getTypeAndId(int type, short id);
    
    public:
        Message(const char *message);
        Message(MessageTypes type, short id, short length, const char *message);
};

#endif