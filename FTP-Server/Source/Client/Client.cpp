#include <fstream>
#include <cstdlib>
#include <sstream>
#include "Client.h"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../Common/json.h"
#include "../Common/static.h"

using namespace std;
using json = nlohmann::json;

Client *Client::instance = nullptr;

Client *Client::getInstance() {
    if (instance == nullptr) instance = new Client();
    return instance;
}

void Client::init(string path) {
    ifstream file(path);
    if (file.is_open()) {

        string data, line;
        while (getline(file, line)) data += line;
        file.close();

        json config = json::parse(data);

        this->dataPort = config["dataChannelPort"];
        this->commandPort = config["commandChannelPort"];

    } else throw invalid_argument("File not found");
}

int Client::connectServer(int port) {
    struct sockaddr_in server_address;
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    if (connect(fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
        throw runtime_error("Error connecting to server");

    return fd;
}

void Client::startCommand() {
    commandFD = connectServer(commandPort);
}

void Client::startData() {
    dataFD = connectServer(dataPort);
    string input = username + " " + password;
    send(dataFD, input.c_str(), input.size(), 0);
}

void Client::sendCommand() {
    string input;
    getline(cin, input);
    send(commandFD, input.c_str(), input.size(), 0);

    string cmd, value;
    stringstream stream(input);
    getline(stream, cmd, ' ');
    if (getline(stream, value, ' ')) {
        if (cmd == "user") username = value;
        else if (cmd == "pass") password = value;
    }

    if (cmd == "ls") receiveDataResponse();
    if ((cmd == "retr") && (!value.empty())) receiveDataResponse();
    if (cmd == "upload") handleUploadCommand(value);
}

void Client::receiveCommandResponse() {
    char buff[BUFFER] = {0};
    recv(commandFD, buff, BUFFER, 0);
    cout << string(buff) << endl;

    if (responseCode(string(buff), 230)) startData();
    if (responseCode(string(buff), 221)) exit(0);
    if (responseCode(string(buff), 123)) throw runtime_error("Unauthorized");
}

bool Client::responseCode(string msg, int code) {
    string _code_;
    stringstream stream(msg);
    getline(stream, _code_, ':');
    if (_code_ == to_string(code)) return true;
    return false;
}

void Client::receiveDataResponse() {
    char buff[BUFFER] = {0};
    recv(dataFD, buff, BUFFER, 0);

    string type;
    stringstream stream((string(buff)));
    getline(stream, type);

    if (type == TYPE_CONSOLE) {
        string line;
        while (getline(stream, line)) cout << line << endl;
    } else if (type == TYPE_FILE) {
        string line, content, path;
        getline(stream, path);
        while (getline(stream, line, '\n')) content += (line + "\n");
        ofstream file(path);
        file << content;
        file.close();
    }

    receiveCommandResponse();
    sendCommand();
}

void Client::handleUploadCommand(string path) {
    try {
        receiveCommandResponse();

        ifstream file(path);
        if (file.is_open()) {
            file.close();

            string data = TYPE_FILE;
            data += "\n";
            data += getFileName(path);
            data += "\n";
            data += getFileContent(path);
            send(dataFD, data.c_str(), data.size(), 0);

            receiveCommandResponse();
        } else {
            cerr << "File not found" << endl;
        }
    } catch (runtime_error &e) {

    }
}

string Client::getFileContent(string path) {
    ifstream file(path);
    string content, line;
    while (getline(file, line)) content += (line + "\n");
    file.close();
    return content;
}

string Client::getFileName(string path) {
    string fileName;
    for (int i = path.size() - 1; i >= 0; i--) {
        if (path[i] == '/') break;
        fileName += path[i];
    }
    reverse(fileName.begin(), fileName.end());
    return fileName;
}
