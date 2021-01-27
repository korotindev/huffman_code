#include <iostream>
#include <sstream>
#include <cassert>

#include "compressor.h"

using namespace std;

void test(string str) {
  istringstream in(str);
  stringstream binary;

  {
    auto tree = BuildHuffmanTreeFrom(in);
    HuffmanCode huffman_code(tree);
    huffman_code.SerializeTo(binary);
    Compressor compressor(huffman_code);
    in.clear();
    in.seekg(0);
    compressor.Encode(in, binary);
  }

  ostringstream out;
  {
    auto code = HuffmanCode::DeserializeFrom(binary);
    Compressor compressor(code);
    compressor.Decode(in, out);
  }

  assert(in.str() == out.str());
}

int main() {
  test("L");
  test("Lorem");
  test("Lorem impsum");
  return 0;
}