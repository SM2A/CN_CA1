#include "Client.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
    try {
        if (argc != 3) invalid_argument("Fix arguments");

        string ip, port;
        istringstream ss(argv[2]);
        getline(ss, ip, ':');
        getline(ss, port);
        Client client = Client(argv[1], ip, stoi(port));
        client.startClient();

    } catch (invalid_argument &e) {
        cerr << e.what() << endl;
    }
}