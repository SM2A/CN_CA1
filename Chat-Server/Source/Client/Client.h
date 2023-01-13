#ifndef CLIENT
#define CLIENT

#include "../Common/Message.h"
#include <string>

using namespace std;

class Client {
    private:
        static unsigned short message_id;
        int sock_fd;
        char *username;
        void sendInitialPackets();
        void getMessages();
        void getInputs();
        void handleList();
        void handleExit();
        void handleSend(int receiver_id, string text);
        void handleListReply(Message &message);
        void handleInfoReply(Message &message);
        void handleReceiveReply(Message &message);
        static unsigned short getMessageId() { return ++message_id; }
    public:
        Client(char *username, string ip, int port);
        void startClient();
};

#endif