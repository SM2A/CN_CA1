SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp, wildcard $(SRC_DIR)/*.h)

all: clean chat-client.out

chat-client.out: $(SRC_FILES)
	${CC} ${SRC_DIR}/*.cpp -o chat-client.out

clean:
	rm -f *.out
