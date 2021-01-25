#include <fstream>
#include <string>

#include "compressor.h"

using namespace std;

namespace {
  void print_help() {
    cout << "Syntax: encode [-h] input_file output_file\n"
         << "options:\n"
         << "  -h    Print this Help.\n";
  }
}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    print_help();
    return 1;
  }
  string filename = argv[1];
  ifstream file(filename);
  if (!file.is_open()) {
    cout << "No such file: " + filename + "\n";
    return 0;
  }

  Compressor compressor(file);
  return 0;
}