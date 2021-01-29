#include <fstream>
#include <string>

#include "compressor.h"

using namespace std;

namespace {
  void print_help() {
    cout << "Syntax: encode [-h] in_file out_file\n"
         << "options:\n"
         << "  -h    Print this Help.\n";
  }
}  // namespace

int main(int argc, char** argv) {
  try {
    if (argc < 2) {
      cout << "Missing in_file\n\n";
      print_help();
      return 1;
    }

    if (argc < 3) {
      cout << "Missing out_file\n\n";
      print_help();
      return 1;
    }

    string in_filename = argv[1];
    ifstream in(in_filename, ios::binary);
    if (!in.is_open()) {
      cout << "No such file: " + in_filename + "\n";
      return 1;
    }

    string out_filename = argv[2];
    ofstream out(out_filename, ios::binary);
    if (!out.is_open()) {
      cout << "No such file: " + out_filename + "\n";
      return 1;
    }

    auto tree = BuildHuffmanTreeFrom(in);
    HuffmanCode huffman_code(tree);
    huffman_code.SerializeTo(out);
    Compressor compressor(huffman_code);
    in.clear();
    in.seekg(0);
    compressor.Encode(in, out);
  } catch (exception& ex) {
    cout << "error: " << ex.what() << "\n";
    print_help();
  }

  return 0;
}