CC=g++
CFLAGS=-std=c++11

build/%: examples/%.cc multi_array.hh
	mkdir -p build
	$(CC) -o $@ $< $(CFLAGS)

all: examples

examples: build/chessboard_indexing build/create_arrays build/vectorize

test: build/test
	build/test

clean:
	rm build/*

build/test: tests/test_main.cc multi_array.hh
	$(CC) $(CFLAGS) -g -o build/test tests/test_main.cc
