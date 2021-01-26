#include <fstream>
#include <string>
#include <iostream>

using namespace std;

namespace {
void print_help() {
  cout << "Syntax: decode [-h] input_file output_file\n"
       << "options:\n"
       << "  -h    Print this Help.\n";
}
}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing input_file\n\n";
    print_help();
    return 1;
  }

  if (argc < 3) {
    cout << "Missing output_file\n\n";
    print_help();
    return 1;
  }

  string input_filename = argv[1];
  ifstream input(input_filename);
  if (!input.is_open()) {
    cout << "No such file: " + input_filename + "\n";
    return 1;
  }

  string output_filename = argv[2];
  ofstream output(output_filename, ios::binary);
  if (!output.is_open()) {
    cout << "No such file: " + output_filename + "\n";
    return 1;
  }

  return 0;
}