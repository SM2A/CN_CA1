#include "../Message.h"
#include <map>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <mutex>
#include <thread>
#include <tuple>

using namespace std;

class Server {
    private:
        map<char[2], pair<char[2] ,vector<Message *> > > stored_messages;
        vector<pair<char[2], int> > online_users;
        vector<tuple<int, char[2], Message *> > received_messages;
        mutex received_messages_mutex;
        int server_fd;
        void startServer();
        void receiveMessage(int socket);
        void analyzeMessage(int socket);
    public:
        Server();
};