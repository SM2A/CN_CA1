#ifndef FTP_SERVER_CLIENT_CLIENT
#define FTP_SERVER_CLIENT_CLIENT

#include <string>

class Client {

private:
    static Client *instance;

    std::string username;
    std::string password;

    int commandPort;
    int dataPort;

    int commandFD;
    int dataFD;

    Client() {}

    int connectServer(int port);
    void startData();
    bool responseCode(std::string msg, int code);
    void receiveDataResponse();
    void handleUploadCommand(std::string path);
    std::string getFileContent(std::string path);
    std::string getFileName(std::string path);

public:
    static Client *getInstance();

    void init(std::string path);
    void startCommand();
    void sendCommand();
    void receiveCommandResponse();
};

#endif
