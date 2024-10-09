CC=g++
CFLAGS=-std=c++11 -Wall -O3

.PHONY: all run clean

all: palgo_server palgo_client libpalgo.so

run: palgo_server
	./palgo_server

clean:
	rm palgo_server palgo_client libpalgo.so

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

.INTERMEDIATE: server.o palgo.o client.o

palgo_server: server.o palgo.o
	$(CC) $^ -o palgo_server

palgo_client: client.o
	$(CC) $^ -o palgo_client

libpalgo.so: palgo.o
	$(CC) $^ -shared -o libpalgo.so
