#ifndef SERVER
#define SERVER

#include "../Message.h"
#include <map>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <mutex>
#include <thread>
#include <tuple>
#include <sys/time.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 

#define PORT 8888

using namespace std;

enum SendStatus {
    SUCCESS,
    FAILURE
};

class Server {
    private:
        unsigned short id = 0;
        vector<tuple<unsigned short, unsigned short, char*> > stored_messages;
        map<int, unsigned short> online_users;
        map<unsigned short, const char*> usernames;
        vector<pair<int, Message> > received_messages;
        mutex received_messages_mutex;
        int server_fd;
        fd_set readfds;
        void handleExit(pair<int, Message> &message);
        void receiveMessage(int socket);
        void getMessage();
        void analyzeMessage(pair<int, Message> &message);
        void acceptConnections();
        void handleConnect(pair<int, Message> &message);
        void handleList(pair<int, Message> &message);
        void handleUserInfo(pair<int, Message> &message);
        void handleSend(pair<int, Message> &message);
        void handleReceive(pair<int, Message> &message);
        void sendUserInfo(int sock, char user_id_char[2], unsigned short message_id);
        unsigned short getId() { return ++id; }
    public:
        Server();
        void startServer();
};

#endif