SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp, wildcard $(SRC_DIR)/*.h)

all: clean web-server.out

web-server.out: $(SRC_FILES)
	${CC} ${SRC_DIR}/*.cpp -o web-server.out

clean:
	rm -f *.out
