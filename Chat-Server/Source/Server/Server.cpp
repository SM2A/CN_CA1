#include "Server.h"
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>

Server::Server()
{
    struct sockaddr_in address;
    int opt = 1;
    
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
    
}

void Server::receiveMessage(int socket)
{
    char buffer[BUFFER_SIZE] = { 0 };
    read(socket, buffer, BUFFER_SIZE);
    received_messages.push_back(make_pair(socket, Message(buffer)));
    
}

void Server::getMessage()
{
    while(true)
    {
        received_messages_mutex.lock();
        if (!received_messages.empty())
        {
            this->analyzeMessage(received_messages[0]);  
            received_messages.erase(received_messages.begin()); 
        }
        received_messages_mutex.unlock();
    }
    
}

void Server::analyzeMessage(pair<int, Message> &message)
{
    MessageTypes type = message.second.getType();
    switch (type)
    {
        case EXIT:
            this->handleExit(message);
            break;
        case CONNECT:
            this->handleConnect(message);
            break;
        case LIST:
            this->handleList(message);
            break;
        case INFO:
            this->handleUserInfo(message);
            break;
        case SEND:
            this->handleSend(message);
            break;
        case RECEIVE:
            this->handleReceive(message);
            break;
        
        default:
            break;
    }
}

void Server::handleExit(pair<int, Message> &message)
{
    auto it = this->online_users.find(message.first);
    if (it != this->online_users.end())
    {
        FD_CLR(message.first, &readfds); 
        this->online_users.erase(it);
    }
    close(message.first);
    shutdown(message.first, SHUT_RDWR);

}

void Server::handleConnect(pair<int, Message> &message)
{
    unsigned short user_id = 0;
    char* text = message.second.getMessage();
    char* username = new char[strlen(text)];
    strcpy(username, text);
    for (auto const& el : this->usernames)
    {
        if (strcmp(el.second ,username) == 0) 
        {
            user_id = el.first;
        }
    }
    if (user_id == 0)
    {
        unsigned short next_id = this->getId();
        usernames.insert(pair<unsigned short, const char*>(next_id, username));
        this->online_users[message.first] = next_id;
    }
    else 
    {
        this->online_users[message.first] = user_id;
    }
    char* reply_message = Message(CONNACK, message.second.getId(), 0, "").getMessagePacket();
    send(message.first, reply_message, strlen(reply_message), 0);
}

void Server::handleList(pair<int, Message> &message)
{
    if (this->online_users.find(message.first) == this->online_users.end()) 
    {
        close(message.first);
    } 
    else 
    {
        char all_users[200] = { 0 };
        int i = 0;
        for (auto const& user : this->online_users)
        {
            if (user.first != message.first) 
            {
                all_users[i++] = (user.second >> 8) & 0xFF;
                all_users[i++] = user.second & 0xFF;
            }
        }
        char* reply_message = Message(LISTREPLY, message.second.getId(), i, all_users).getMessagePacket();
        send(message.first, reply_message, i + 2, 0);
    }
}

void Server::sendUserInfo(int sock, char *user_id_char, unsigned short message_id)
{

    unsigned short user_id = (short)user_id_char[1] + (short)(user_id_char[0] << 8);
    if (this->usernames.find(user_id) == this->usernames.end())
    {
        char* reply_message = Message(INFOREPLY, message_id, 0, "").getMessagePacket();;
        send(sock, reply_message, strlen(reply_message), 0);
    }
    else
    {
        char* reply_message = Message(INFOREPLY, message_id, strlen(this->usernames[user_id]), this->usernames[user_id]).getMessagePacket();
        send(sock, reply_message, strlen(reply_message), 0);
    }
}

void Server::handleUserInfo(pair<int, Message> &message)
{
    if (this->online_users.find(message.first) == this->online_users.end()) 
    {
        close(message.first);
    } 
    else 
    {
        sendUserInfo(message.first, message.second.getMessage(), message.second.getId());
    }
}

void Server::handleSend(pair<int, Message> &message)
{
    if (this->online_users.find(message.first) == this->online_users.end()) 
    {
        close(message.first);
    } 
    else 
    {
        char *text = message.second.getMessage();
        unsigned short user_id = (short)text[1] + (short)(text[0] << 8);
        short length = message.second.getLength();
        char *message_text = new char[length];
        message_text[0] = (online_users[message.first] >> 8) & 0xFF;
        message_text[1] = online_users[message.first] & 0xFF;
        
        for (int i = 2; i < length; i++)
        {
            message_text[i] = text[i];
        }
        for (auto const& user : this->online_users)
        {
            if (user.second == user_id) 
            {
                char* send_message =  Message(RECEIVEREPLY, message.second.getId(), length, message_text).getMessagePacket();
                send(user.first, send_message, 3 + length, 0);
                sendUserInfo(user.first, message_text, message.second.getId());
                char* reply_message =  Message(SENDREPLY, message.second.getId(), 1, static_cast<char>(SUCCESS)).getMessagePacket();
                send(message.first, reply_message, 3, 0);
                return;
            }
        }
        char* save_message= new char[length - 2];
        for (int i = 0; i < length - 2; i++)
        {
            save_message[i] = text[i + 2];
        }
        stored_messages.push_back(make_tuple(user_id, online_users[message.first], save_message));
        char* reply_failed_message =  Message(SENDREPLY, message.second.getId(), 1, static_cast<char>(FAILURE)).getMessagePacket();
        send(message.first, reply_failed_message, 3, 0);

    }
}

void Server::handleReceive(pair<int, Message> &message)
{
    if (this->online_users.find(message.first) == this->online_users.end()) 
    {
        close(message.first);
    } 
    else 
    {
        vector<int> to_delete_idxs;
        unsigned short user_id = this->online_users[message.first];
        for (int i = 0; i < this->stored_messages.size(); i++)
        {
            if (user_id == get<0>(this->stored_messages[i])) 
            {
                char *text = get<2>(this->stored_messages[i]);
                unsigned short sender_id = get<1>(this->stored_messages[i]);
                char message_text[3 + strlen(text)] = { 0 };
                message_text[0] = (sender_id >> 8) & 0xFF;
                message_text[1] = sender_id & 0xFF;
                for (int j = 0; j < strlen(text); j++)
                {
                    message_text[j + 2] = text[j];
                }
                to_delete_idxs.push_back(i);
                char* reply_message =  Message(RECEIVEREPLY, 0, 3 + strlen(text), message_text).getMessagePacket();
                sleep(0.3);
                send(message.first, reply_message, 5 + strlen(text), 0);
                sendUserInfo(message.first, message_text, message.second.getId());
            }
        }

        for (int i = to_delete_idxs.size() - 1; i >=0; i--)
        {
            this->stored_messages.erase(this->stored_messages.begin() + to_delete_idxs[i]);
        }

    }
}

void Server::acceptConnections() 
{
    while(true)
    {
        FD_ZERO(&readfds);
        FD_SET(this->server_fd, &readfds);
        struct sockaddr_in address;
        int new_socket, addrlen = sizeof(address), max_sd = this->server_fd;

        received_messages_mutex.lock();
        for (auto const &el : this->online_users)
        {
            FD_SET(el.first, &readfds);
            if (el.first > max_sd)
            {
                max_sd = el.first;

            }
        }
        received_messages_mutex.unlock();
        int activity = select( max_sd + 1, &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno != EINTR))  
        {  
            cerr<<"select error";
            exit(EXIT_FAILURE);  
        }
        if (FD_ISSET(this->server_fd, &readfds))
        {
            if ((new_socket
                = accept(server_fd, (struct sockaddr*)&address,
                        (socklen_t*)&addrlen))
                < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            this->online_users[new_socket] = 0;
        }
        received_messages_mutex.lock();
        for (auto const &el : this->online_users)
        {
            if (FD_ISSET(el.first, &readfds))
            {
                this->receiveMessage(el.first);
            }
        }
        received_messages_mutex.unlock();
    }
}

void Server::startServer() 
{
    thread gm(&Server::getMessage, this);
    thread ac(&Server::acceptConnections, this);
    
    gm.join();
    ac.join();

    shutdown(this->server_fd, SHUT_RDWR);
}

