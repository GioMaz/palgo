CC=g++
CFLAGS=-std=c++11 -Wall -O3

libpalgo.so: palgo.o api.o
	$(CC) $^ -shared -o libpalgo.so

.INTERMEDIATE: palgo.o api.o

%.o: %.cc
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm libpalgo.so
