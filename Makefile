CC=g++
CFLAGS=-std=c++11 -Wall -O3
PALGODIR=./palgo

.PHONY: all run clean

all: palgo_server palgo_client

run: palgo_server
	./palgo_server

clean:
	rm palgo_server palgo_client database.sqlite

palgo_server: $(PALGODIR)/server.cc $(PALGODIR)/palgo.cc
	$(CC) $(CFLAGS) $^ -o palgo_server

palgo_client: $(PALGODIR)/client.cc
	$(CC) $(CFLAGS) $^ -o palgo_client
