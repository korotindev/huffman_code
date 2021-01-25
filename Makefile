CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -Werror -std=c++14
TARGETS = encode decode

.PHONY: all
all: build build/encode build/decode

build:
	mkdir -p build

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/encode: build/encode.o build/compressor.o build/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o $@ $^

build/decode: build/decode.o build/compressor.o build/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf build/*

.PHONY: format
format:
	clang-format --style=Google --sort-includes -i src/*