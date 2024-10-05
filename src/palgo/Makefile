CC=g++
CFLAGS=-std=c++11 -Wall -O3

.PHONY: all run clean

all: palgo_server palgo_client

run: palgo_server
	./palgo_server

clean:
	rm palgo_server palgo_client

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

.INTERMEDIATE: server.o palgo.o client.o

palgo_server: server.o palgo.o
	$(CC) $^ -o palgo_server

palgo_client: client.o
	$(CC) $^ -o palgo_client
