#include <iostream>
#include "HttpServer.h"

using namespace std;

int main(int argc, char **argv) {

    HttpServer *server = HttpServer::getInstance();

    try {
        if (argc == 2) server->init(string(argv[1]));
        else if (argc < 2) throw invalid_argument("No port number entered");
        else if (argc > 2) throw invalid_argument("Fix arguments");

        server->startServer();

    } catch (invalid_argument &e) {
        cerr << e.what() << endl;
    }

    return 0;
}
