#include "Server.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#define PORT 8888
#define BUFFER_SIZE (1024 * 10)

Server::Server()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char* hello = "Hello from server";
 
    
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr<<"socket failed"<<endl;
        exit(EXIT_FAILURE);
    }
 
    
    if (setsockopt(this->server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        cerr<<"setsockopt"<<endl;
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    if (bind(this->server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        cerr<<"bind failed"<<endl;
        exit(EXIT_FAILURE);
    }
    if (listen(this->server_fd, 3) < 0) {
        cerr<<"listen"<<endl;
        exit(EXIT_FAILURE);
    }
    //send(new_socket, hello, strlen(hello), 0);
    
}

void Server::receiveMessage(int socket)
{
    while(true)
    {
        char buffer[BUFFER_SIZE] = { 0 };
        read(new_socket, buffer, BUFFER_SIZE);

    }
    close(new_socket);
}

void Server::analyzeMessage(int socket)
{

}

void Server::startServer() 
{
    vector<thread> rm;
    thread am(&Server::analyzeMessage, this);
    while(true)
    {
        struct sockaddr_in address;
        int new_socket, addrlen = sizeof(address);
        if ((new_socket
            = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        rm.push_back(thread(&Server::receiveMessage, this, new_socket));
    }

    for(auto &el : rm)
    {
        el.join();
    }
    am.join();
    // closing the listening socket
    shutdown(this->server_fd, SHUT_RDWR);
}

