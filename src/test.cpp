#include <iostream>
#include <sstream>

#include "compressor.h"
#include "utils.h"

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

  in.clear();
  in.seekg(0);
  binary.clear();
  binary.seekg(0);
  ostringstream out;
  {
    auto code = HuffmanCode::DeserializeFrom(binary);
    Compressor compressor(code);
    compressor.Decode(binary, out);
  }

  if (in.str() != out.str()) {
    cout << "Test failed\n"
         << "Expected: " << in.str() << "\nResult: " << out.str() << "\n\n";
  }
}

int main() {
  test("Lorem");
  return 0;
}