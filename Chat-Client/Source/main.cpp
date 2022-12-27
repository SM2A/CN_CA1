#include <vector>
#include <bitset>
#include <cstring>
#include <csignal>
#include <iostream>
#include <arpa/inet.h>

#define BUFFER (1024 * 1024)

using namespace std;

void continue_loop(int sig) {}
int connectToServer(const char *address, const char *name);

int main(int argc, char **argv) {

    try {
        int fd;
        if (argc == 3) {
            fd = connectToServer(argv[1], argv[2]);
        } else if (argc < 3) throw invalid_argument("Not all arguments entered");
        else throw invalid_argument("Too many arguments");

        signal(SIGALRM, continue_loop);
        siginterrupt(SIGALRM, 1);

        while (true) {
            alarm(3);
            char input[1024];
            read(0, input, 1024);
            char command[5] = {input[0], input[1], input[2], input[3], '\n'};
            if (strcmp(command, "exit\n") == 0) {
                return 0;
            } else if (strcmp(command, "list\n") == 0) {
                char message[BUFFER];
                sprintf(message, "001000000000010\n");
                send(fd, message, strlen(message), 0);
                char buff[BUFFER] = {0};
                recv(fd, buff, BUFFER, 0);
                char payload_size[9];
                memcpy(payload_size, &buff[8], 8);
                payload_size[8] = '\0';
                int size = (stoi(payload_size) - 2) / 2;
                vector<string> users;
                for (int i = 0; i < size; i += 16) {
                    char user[17];
                    memcpy(user, &buff[i], 16);
                    user[16] = '\0';
                    users.push_back(string(user));
                }
                for (string user: users) cout << "\t- " << user << endl;
            } else if (strcmp(command, "send\n") == 0) {
                input[0] = '-';
                input[1] = '-';
                input[2] = '-';
                input[3] = '-';
                input[4] = '-';
                string _input_ = string(input);
                string user_id = _input_.substr(5, _input_.find(' '));
                string msg = _input_.substr(_input_.find(' ') + 1, _input_.length());

                char message[BUFFER];
                string user = bitset<8>(user_id.size()).to_string();
                string length = bitset<8>(user_id.size() + msg.size() + 4).to_string();
                sprintf(message, "01110000%s%s%s\n", length.c_str(), user.c_str(), msg.c_str());
                send(fd, message, strlen(message), 0);
                char buff[BUFFER] = {0};
                recv(fd, buff, BUFFER, 0);
                if (buff[8] == '1') {
                    cout << "Message sent successfully" << endl;
                } else {
                    cout << "Message sending failed" << endl;
                }
            } else {
                char buff[BUFFER] = {0};
                alarm(3);
                recv(fd, buff, BUFFER, 0);
                alarm(0);
                if (string(buff).length() > 0) cout << buff << endl;
            }
            input[0] = 0;
        }

    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

int connectToServer(const char *address, const char *name) {

    string _address_ = string(address);
    string host = _address_.substr(0, _address_.find(':'));
    int port = stoi(_address_.substr(_address_.find(':') + 1, _address_.length()));

    struct sockaddr_in server_address;

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(host.c_str());

    if (connect(fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        throw runtime_error("Error connecting to server");
    }

    char connect[1024];
    string binary = bitset<8>(strlen(name) + 2).to_string();
    sprintf(connect, "00010000%s%s\n", binary.c_str(), name);
    send(fd, connect, strlen(connect), 0);
    char buff[BUFFER] = {0};
    recv(fd, buff, BUFFER, 0);

    return fd;
}
