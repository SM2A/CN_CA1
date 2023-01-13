SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES_S := $(wildcard $(SRC_DIR)/*.cpp, wildcard $(SRC_DIR)/*.h, wildcard $(SRC_DIR)/Server/*.cpp, wildcard $(SRC_DIR)/Server/*.h)
SRC_FILES_C := $(wildcard $(SRC_DIR)/Client/*.cpp, wildcard $(SRC_DIR)/Client/*.h, wildcard $(SRC_DIR)/*.cpp, wildcard $(SRC_DIR)/*.h)

all: clean chat-server.out chat-client.out

chat-server.out: $(SRC_FILES_S)
	${CC} ${SRC_DIR}/Server/*.cpp -pthread -o chat-server.out

chat-client.out: $(SRC_FILES_C)
	${CC} ${SRC_DIR}/Client/*.cpp -o chat-client.out

clean:
	rm -f *.out