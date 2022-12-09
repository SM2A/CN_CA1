#ifndef WEB_SERVER_HTTPSERVER_H
#define WEB_SERVER_HTTPSERVER_H

#include <string>
#include <arpa/inet.h>

class HttpServer {

private:

    const std::string SERVER_ADDRESS = "127.0.0.1";

    int serverFD;
    struct sockaddr_in socketAddress;

    static HttpServer *instance;

    std::string response(const std::string &request);

public:

    static HttpServer *getInstance();

    void init(const std::string &port);
    void startServer();

};

#endif
