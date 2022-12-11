project(Web-Server)

SET (PROJECT_ROOT "Web-Server/Source")

add_executable(web-server
        ${PROJECT_ROOT}/main.cpp
        ${PROJECT_ROOT}/HttpServer.cpp
        ${PROJECT_ROOT}/HttpServer.h
        ${PROJECT_ROOT}/Util.h)
