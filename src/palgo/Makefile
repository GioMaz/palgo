CC=g++
CFLAGS=-std=c++11 -Wall -O3

.PHONY: all run clean

all: palgo_server palgo_client

run: palgo_server
	./palgo_server

clean:
	rm palgo_server palgo_client database.sqlite

palgo_server: server.cc palgo.cc
	$(CC) $(CFLAGS) $^ -o palgo_server

palgo_client: client.cc
	$(CC) $(CFLAGS) $^ -o palgo_client
