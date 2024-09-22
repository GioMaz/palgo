PALGODIR=./palgo

.PHONY: run clean

run: index.js palgo.js
	node index.js

palgo.js: $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc
	emcc -lembind -o palgo.js $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc -s MODULARIZE=1

clean:
	rm palgo.js palgo.wasm
