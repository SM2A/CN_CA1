SRC_DIR=Source

COMPILER = g++
VERSION = -std=c++11
CC = ${COMPILER} ${VERSION}

SRC_FILES_S := $(wildcard $(SRC_DIR)/Server/*.cpp, wildcard $(SRC_DIR)/Server/*.h, wildcard $(SRC_DIR)/Common/*.cpp, wildcard $(SRC_DIR)/Common/*.h)
SRC_FILES_C := $(wildcard $(SRC_DIR)/Client/*.cpp, wildcard $(SRC_DIR)/Client/*.h, wildcard $(SRC_DIR)/Common/*.cpp, wildcard $(SRC_DIR)/Common/*.h)

all: clean ftp-server.out ftp-client.out

ftp-server.out: $(SRC_FILES_S)
	${CC} ${SRC_DIR}/Server/*.cpp -pthread -o ftp-ftp-server.out

ftp-client.out: $(SRC_FILES_C)
	${CC} ${SRC_DIR}/Client/*.cpp -o ftp-client.out

clean:
	rm -f *.out
