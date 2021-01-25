CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -Werror
TARGETS = encode decode

.PHONY: all
all: build build/encode build/decode

build:
	mkdir -p build

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

build/encode: build/encode.o build/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o $@ $^

build/decode: build/decode.o build/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf **/*.o build/*