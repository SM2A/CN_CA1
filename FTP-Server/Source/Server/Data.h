#ifndef FTP_SERVER_CLIENT_DATA
#define FTP_SERVER_CLIENT_DATA

#include <string>

class Data {

private:
    static void sendData(int fd, std::string data);
    static void receiveData(int fd, std::string name);

    friend class Server;

};

#endif
