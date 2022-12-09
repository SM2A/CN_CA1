#include "Util.h"
#include <unistd.h>
#include "HttpServer.h"

using namespace std;

#define BUFFER (1024*1024)

HttpServer *HttpServer::instance = nullptr;

HttpServer *HttpServer::getInstance() {
    if (instance == nullptr) instance = new HttpServer();
    return instance;
}

void HttpServer::init(const string &port) {
    serverFD = socket(AF_INET, SOCK_STREAM, 0);

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr(SERVER_ADDRESS.c_str());

    try {
        socketAddress.sin_port = htons(stoi(port));
    } catch (invalid_argument &e) {
        throw invalid_argument("Invalid port number");
    }

    bind(serverFD, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
}

void HttpServer::startServer() {
    listen(serverFD, 10);

    while (true) {

        unsigned int socketLen = sizeof(socketAddress);
        int requestFD = accept(serverFD, (sockaddr *) &socketAddress, &socketLen);

        char buffer[BUFFER] = {0};
        read(requestFD, buffer, BUFFER);

        cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
        cout << buffer << endl;
        cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

        string responseData = response(string(buffer));
        write(requestFD, responseData.c_str(), responseData.size());

        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
        cout << responseData << endl;
        cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

        close(requestFD);
    }
}
