LIBPALGO=./libpalgo

libpalgo.so:
	make $@ -C $(LIBPALGO)
	mv $(LIBPALGO)/libpalgo.so .

.PHONY: clean

run: libpalgo.so
	python3 main.py

clean:
	rm libpalgo.so
