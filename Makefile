PALGODIR=./palgo

.PHONY: run clean

run: index.js output.js
	node index.js

output.js: $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc
	emcc -lembind -o output.js $(PALGODIR)/palgo.cc $(PALGODIR)/bind.cc -s MODULARIZE=1

clean:
	rm output.js output.wasm
