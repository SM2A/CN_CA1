#include <iostream>
#include <arpa/inet.h>
#include <csignal>
#include <climits>
#include <cstring>
#include <vector>

#define BUFFER (1024 * 1024)

using namespace std;

int connectToServer(const char *address);

void continue_loop(int sig) {}


string name;

int main(int argc, char **argv) {

    try {
        int fd;
        if (argc == 3) {
            name = string(argv[2]);
            fd = connectToServer(argv[1]);
        } else if (argc < 3) throw invalid_argument("Not all arguments entered");
        else throw invalid_argument("Too many arguments");

        signal(SIGALRM, continue_loop);
        siginterrupt(SIGALRM, 1);

        while (true) {
            alarm(3);
            char input[1024];
            read(0, input, 1024);
            if (strcmp(input, "Exit\n") == 0) {
                return 0;
            } else if (strcmp(input, "List\n") == 0) {
                sprintf(input, "001000000000010\n");
                char buff[BUFFER] = {0};
                recv(fd, buff, BUFFER, 0);
                char payload_size[9];
                memcpy(payload_size, &buff[8], 8);
                payload_size[8] = '\0';
                int size = (stoi(payload_size) - 2) / 2;
                vector<string> users;
                for (int i = 0; i < size; i+=16) {
                    char user[17];
                    memcpy(user, &buff[i], 16);
                    user[16] = '\0';
                    users.push_back(string(user));
                }
                for (string user: users) {
                    cout << "\t- " << user << endl;
                }
            }
            send(fd, input, strlen(input), 0);
            input[0] = 0;


            char buff[BUFFER] = {0};
            alarm(3);
            recv(fd, buff, BUFFER, 0);
            alarm(0);
            if (string(buff).length() > 0) cout << buff << endl;
        }

    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}

int connectToServer(const char *address) {

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

    return fd;
}
