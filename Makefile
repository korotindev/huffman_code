CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -Werror

LIBS = huffman_tree
TARGETS = encode decode

all: prepare $(TARGETS)

prepare:
	mkdir -p build/lib
	mkdir -p build/obj
	mkdir -p build/bin

build/obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

encode: build/obj/encode.o build/obj/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o build/bin/$@ $^

decode: build/obj/decode.o build/obj/huffman_tree.o
	$(CXX) $(CXXFLAGS) -o build/bin/$@ $^

clean:
	rm -rf build/**/*.o build/bin/* encode decode