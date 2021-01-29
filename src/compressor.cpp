#include "compressor.h"

#include <algorithm>
#include <bitset>

#include "cached_bit_reader.h"
#include "cached_bit_writer.h"

using namespace std;

namespace {
  uint ReverseNLastBits(uint code, uint n) {
    uint result = 0;
    for (uint i = 0; i < n; i++) {
      result = result << 1;
      result |= (code & 1);
      code = code >> 1;
    }
    code = code << n;
    return result | code;
  }
}  // namespace

Compressor::Compressor(const HuffmanCode& huffman_code) : huffman_code_(huffman_code) {}

void Compressor::Encode(istream& in, ostream& out) const {
  CachedBitWriter bit_writer(out);
  char c;
  while (in.get(c)) {
    const auto& compression_info = huffman_code_.GetSymbolCompressionInfo(c);
    uint reversed_code = ReverseNLastBits(compression_info.code, compression_info.length);
    bit_writer.WriteNLastBits(reversed_code, compression_info.length);
  }
  bit_writer.Flush();
}

void Compressor::Decode(istream& in, ostream& out) const {
  CachedBitReader bit_reader(in);
  uint code = 0;
  uint code_length = 0;
  unsigned long long symbols_readed = 0;
  unsigned long long symbols_needed = huffman_code_.GetSymbolsCount();
  bit_reader.CacheNextChunk();

  while (symbols_readed < symbols_needed && bit_reader.HasBits()) {
    code = code << 1;
    code |= bit_reader.ReadBit();
    code_length++;
    auto sym_ptr = huffman_code_.FindSymbolByCode(HuffmanCode::SymbolInfo{code, code_length});
    if (sym_ptr) {
      out << *sym_ptr;
      code = 0;
      code_length = 0;
      symbols_readed++;
    }
  }
}
