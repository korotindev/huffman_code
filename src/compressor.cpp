#include "compressor.h"

#include <sstream>
#include <unordered_map>

#include "huffman_tree.h"

using namespace std;

namespace {
using CharStat = unordered_map<char, unsigned long long>;

CharStat calc_symbol_stats(istream& in) {
  CharStat stat;
  char sym = 0;
  while (in.get(sym)) {
    stat[sym]++;
  }
  return stat;
}

class BitWriter {
  ostream& out;
  unsigned char byte = 0;
  unsigned char counter = 0;

  void Flush() {
    out << byte;
    byte = 0;
    counter = 0;
  }

 public:
  explicit BitWriter(ostream& out) : out(out) {}

  void WriteBit(int bit) {
    byte |= (bit << counter);
    if (++counter == 8) {
      Flush();
    }
  }

  void Finish() {
    while (byte) {
      WriteBit(0);
    }
  }
};
}  // namespace

void Compress(istream& input, ostream& output) {
  auto symbol_stat = calc_symbol_stats(input);
  HuffmanTree tree(symbol_stat);
  auto symbol_bitmask = tree.BuildSymBitmask();
  for (const auto& kv : symbol_bitmask) {
    output << kv.first << ' ' << kv.second << ' ';
  }

  output << "\n";

  BitWriter writer(output);

  char sym = 0;
  input.clear();
  input.seekg(0);
  while (input.get(sym)) {
    for (const auto& bit : symbol_bitmask[sym]) {
      if (bit == '0') {
        writer.WriteBit(0);
      } else {
        writer.WriteBit(1);
      }
    }
  }

  writer.Finish();
}

namespace {

unordered_map<string, char> read_alphabet(istream& input) {
  unordered_map<string, char> result;
  string alphabet;
  getline(input, alphabet);
  stringstream alphabet_input(alphabet);
  char sym;
  string bitmask;
  while (alphabet_input >> sym) {
    alphabet_input >> bitmask;
    result[move(bitmask)] = sym;
  }
  return result;
}

class BitReader {
  unsigned char byte = 0;
  unsigned char counter = 0;
  istream& in;

 public:
  BitReader(istream& in) : in(in) {}

  bool has_data() { return in || (byte != 0); }

  int read() {
    if (counter == 0) {
      in >> byte;
    } else {
      byte = byte >> 1;
    }
    if (++counter == 8) {
      counter = 0;
    }
    return byte & 1;
  }
};
}  // namespace

void Decompress(istream& input, ostream& output) {
  auto alphabet = read_alphabet(input);
  BitReader reader(input);
  string code;
  while (reader.has_data()) {
    code.push_back(reader.read() == 1 ? '1' : '0');
    auto it = alphabet.find(code);
    if (it != alphabet.end()) {
      output << it->second;
      code.clear();
    }
  }
}