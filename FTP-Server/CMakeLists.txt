project(FTP-Server-Client)

SET (PROJECT_ROOT "FTP-Server/Source")
SET (SERVER_DIR "${PROJECT_ROOT}/Server")
SET (CLIENT_DIR "${PROJECT_ROOT}/Client")
SET (COMMON_DIR "${PROJECT_ROOT}/Common")

add_executable(ftp-server
        ${SERVER_DIR}/main.cpp
        ${COMMON_DIR}/static.h
        ${SERVER_DIR}/CommandExecutor.cpp
        ${SERVER_DIR}/CommandExecutor.h
        ${SERVER_DIR}/User.cpp
        ${SERVER_DIR}/User.h
        ${SERVER_DIR}/Server.cpp
        ${SERVER_DIR}/Server.h
        ${COMMON_DIR}/json.h
        ${SERVER_DIR}/Logger.cpp
        ${SERVER_DIR}/Logger.h
        ${SERVER_DIR}/Command.cpp
        ${SERVER_DIR}/Command.h
        ${SERVER_DIR}/Data.cpp
        ${SERVER_DIR}/Data.h)

add_executable(ftp-client
        ${CLIENT_DIR}/main.cpp
        ${COMMON_DIR}/static.h
        ${CLIENT_DIR}/Client.cpp
        ${CLIENT_DIR}/Client.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ftp-server PRIVATE Threads::Threads)
