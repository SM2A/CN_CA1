project(Chat-Server)

SET (PROJECT_ROOT "Chat-Server/Source")
SET (SERVER_DIR "${PROJECT_ROOT}/Server")
SET (CLIENT_DIR "${PROJECT_ROOT}/Client")
SET (COMMON_DIR "${PROJECT_ROOT}/Common")

add_executable(chat-server
        ${SERVER_DIR}/main.cpp
        ${SERVER_DIR}/Server.cpp
        ${SERVER_DIR}/Server.h
        ${COMMON_DIR}/Message.cpp
        ${COMMON_DIR}/Message.h)

add_executable(chat-client
        ${CLIENT_DIR}/main.cpp
        ${CLIENT_DIR}/Client.cpp
        ${CLIENT_DIR}/Client.h
        ${COMMON_DIR}/Message.cpp
        ${COMMON_DIR}/Message.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(chat-server PRIVATE Threads::Threads)
target_link_libraries(chat-client PRIVATE Threads::Threads)
