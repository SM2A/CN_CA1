#include "HttpServer.h"
#include "Util.h"
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <algorithm>

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

        cout << buffer << endl;
        cout << "---------------------------------------------------------------------------------------------" << endl;

        string response = this->response(string(buffer));
        write(requestFD, response.c_str(), response.size());

        close(requestFD);
    }
}

string HttpServer::response(const string &request) {
    string path = requestPath(request);
    string data = generateResponse(path);
    cout << data << endl;
    return data;
}
