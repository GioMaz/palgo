PALGODIR=./palgo

run: index.js palgo.js
	node index.js

palgo.js: $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc
	emcc -lembind -o palgo.js $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc -s MODULARIZE=1

.PHONY: run

clean:
	rm palgo.js palgo.wasm
