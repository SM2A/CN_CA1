#ifndef WEB_SERVER_UTIL_H
#define WEB_SERVER_UTIL_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#define RES_PATH std::string("../Web-Server/Res")

std::string requestPath(const std::string &request) {
    std::string method, path;
    std::stringstream stream = std::stringstream(request);

    getline(stream, method, ' ');
    getline(stream, path, ' ');

    return path;
}

std::string fileExtension(const std::string &path) {
    std::string extension;
    for (int i = path.size() - 1; i >= 0; i--) {
        if (path[i] == '.') break;
        extension += path[i];
    }
    reverse(extension.begin(), extension.end());
    return extension;
}

std::string contentType(const std::string &extension) {
    std::string type;
    if (extension == "html") type = "text/html";
    else if (extension == "gif") type = "image/gif";
    else if (extension == "jpeg") type = "image/jpeg";
    else if (extension == "mp3") type = "audio/mpeg";
    else if (extension == "pdf") type = "application/pdf";
    return "Content-Type: " + type + "\n";
}

std::string generateResponse(const std::string &_path_) {

    std::string path;
    if (_path_ == "/") path = (RES_PATH + "/index.html");
    else path = (RES_PATH + _path_);

    std::string extension = fileExtension(path);
    std::ifstream file(path);

    std::stringstream fileBuffer, data;
    fileBuffer << file.rdbuf();
    file.close();

    data << "HTTP/1.1 200 OK\n"
         << contentType(extension)
         << "Content-Length: " << fileBuffer.str().size() << "\n\n"
         << fileBuffer.str();

    return data.str();
}

#endif
