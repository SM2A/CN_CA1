#include "Client.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>

using namespace std;

Client::Client(char *username, string ip, int port)
{
    int  client_fd;
    struct sockaddr_in serv_addr;
    if ((this->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        exit(EXIT_FAILURE);
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
 
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        exit(EXIT_FAILURE);
    }
 
    if ((client_fd
         = connect(this->sock_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }
    this->username = username;
    this->sendInitialPackets();
}

void Client::sendInitialPackets()
{
    char *connect_message = Message(CONNECT, 0, strlen(this->username), username).getMessagePacket();
    send(this->sock_fd, connect_message, strlen(connect_message), 0);
    sleep(0.1);
    char *receive_message = Message(RECEIVE, 0, 0, "").getMessagePacket();
    send(this->sock_fd, receive_message, 2, 0);
}

void Client::startClient()
{
    thread gm(&Client::getMessages, this);
    thread gi(&Client::getInputs, this);
    
    gm.join();
    gi.join();
}

void Client::getMessages() 
{
    while (true)
    {
        char buffer[BUFFER_SIZE] = { 0 };
        char *begin = buffer;
        char *end = begin + BUFFER_SIZE;
        fill(begin, end, 0);
        read(this->sock_fd, buffer, BUFFER_SIZE);
        Message message = Message(buffer);
        switch (message.getType())
        {
            case LISTREPLY:
                this->handleListReply(message);
                break;

            case INFOREPLY:
                this->handleInfoReply(message);
                break;
            case RECEIVEREPLY:
                this->handleReceiveReply(message);
                break;
            default:
                break;
        }
    }
}

void Client::handleListReply(Message &message)
{
    char *all_id = message.getMessage();
    if (message.getLength() == 0) 
    {
        cout<<"There isn't any other online user"<<endl;
    }
    for (int i = 0; i < message.getLength(); i += 2)
    {
        char user_id[2];
        user_id[0] = all_id[i];
        user_id[1] = all_id[i + 1];
        char *info_message = Message(INFO, 0, 2, user_id).getMessagePacket();
        send(this->sock_fd, info_message, 4, 0);
    }
}

void Client::handleInfoReply(Message &message)
{
    cout<<" - "<<message.getMessage()<<endl;
}

void Client::handleReceiveReply(Message &message)
{
    char buffer[BUFFER_SIZE] = { 0 };
    char *begin = buffer;
    char *end = begin + BUFFER_SIZE;
    fill(begin, end, 0);
    char* text = message.getMessage();
    text += 2;
    read(this->sock_fd, buffer, BUFFER_SIZE);
    Message sender = Message(buffer);
    string username = sender.getMessage();
    cout<<" - "<<username<<": "<<text<<endl;
}

void Client::getInputs()
{
    while (true)
    {
        string input;
        getline(cin, input);
        if (input == "list")
        {
            this->handleList();
        }
        else if (input == "exit")
        {
            this->handleExit();
        }
        else
        {
            string command, receiver_id, text;
            istringstream ss(input);
            getline(ss, command, ' ');
            if (command == "send")
            {
                getline(ss, receiver_id, ' ');
                getline(ss, text);
                this->handleSend(stoi(receiver_id), text);
            }
        }
    }
}

void Client::handleList()
{
    char *list_message = Message(LIST, 0, 0, "").getMessagePacket();
    send(this->sock_fd, list_message, strlen(list_message), 0);
}

void Client::handleExit()
{
    char *exit_message = Message(EXIT, 0, 0, "").getMessagePacket();
    send(this->sock_fd, exit_message, strlen(exit_message), 0);
    exit(EXIT_SUCCESS);
}

void Client::handleSend(int receiver_id, string text)
{
    char message[text.size() + 3] = { 0 };
    message[0] = (receiver_id >> 8) & 0xFF;
    message[1] = receiver_id & 0xFF;
    for (int i = 0; i < text.size(); i++)
    {
        message[i + 2] = text[i];
    }
    char *send_message = Message(SEND, 0, text.size() + 3, message).getMessagePacket();
    send(this->sock_fd, send_message, text.size() + 5, 0);
}