#include "Data.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/socket.h>
#include "../Common/static.h"

using namespace std;

void Data::sendData(int fd, string data) {
    send(fd, data.c_str(), data.size(), 0);
}

void Data::receiveData(int fd, string name) {
    mkdir(string("Files").c_str(), 0777);

    char buff[BUFFER] = {0};
    recv(fd, buff, BUFFER, 0);

    string type;
    stringstream stream((string(buff)));
    getline(stream, type);

    string line, content, path;
    getline(stream, path);
    while (getline(stream, line, '\n')) content += (line + "\n");
    ofstream file("Files/" + name);
    file << content;
    file.close();
}
