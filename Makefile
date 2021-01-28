CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Werror -std=c++14 -g -O0 -fsanitize=address
TARGETS = encode decode
SDK_OBJECTS = build/compressor.o build/huffman_tree.o build/huffman_code.o build/cached_bit_writer.o build/cached_bit_reader.o

.PHONY: all
all: build build/encode build/decode build/test

build:
	mkdir -p build

build/%.o: src/%.cpp 
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/encode: build/encode.o $(SDK_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/decode: build/decode.o $(SDK_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/test: build/test.o $(SDK_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf build/*

.PHONY: format
format:
	clang-format --sort-includes -i src/*

.PHONY: test
test: build/test
	build/test