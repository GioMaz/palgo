CC=g++
CFLAGS=-std=c++11 -Wall -O3
PALGODIR=./palgo

palgo_server: $(PALGODIR)/server.cc $(PALGODIR)/palgo.cc
	$(CC) $(CFLAGS) $^ -o palgo_server

.PHONY: clean

run: palgo_server
	./palgo_server

clean:
	rm palgo_server database.sqlite

