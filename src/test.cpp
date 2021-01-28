#include <iostream>
#include <sstream>

#include "compressor.h"
#include "utils.h"

using namespace std;

void test(string str) {
  cout << "Test \"" << str << "\": ";
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
    cout << "FAILED\n"
         << "Expected: " << in.str() << " (size: " << in.str().size() << ")\n"
         << "Result: " << out.str() << " (size: " << out.str().size() << ")\n\n";
  } else {
    cout << "OK\n";
  }
}

int main() {
  test("L");
  test("Lorem");
  test("Lorem impsum");
  test("Lorem impsum dolor sit amet");
  return 0;
}