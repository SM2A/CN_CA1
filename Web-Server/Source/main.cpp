#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main() {

    struct sockaddr_in address;
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int optCommand = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optCommand, sizeof(optCommand));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(string("127.0.0.1").c_str());
    address.sin_port = htons(1380);

    bind(fd, (struct sockaddr *) &address, sizeof(address));

    listen(fd, 10);

    while (true) {

        unsigned int addrlen = sizeof(address);
        int new_fd = accept(fd, (sockaddr *) &address, &addrlen);

        char buffer[40000] = {0};
        read(new_fd, buffer, 40000);

        cout << buffer << endl;
        cout << "---------------------------------------------------------------" << endl;

//        write(new_fd, buildResponse().c_str(), buildResponse().size());

        close(new_fd);
    }


    return 0;
}
