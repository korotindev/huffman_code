#include "compressor.h"

#include "cached_bit_writer.h"
#include "cached_bit_reader.h"

using namespace std;

Compressor::Compressor(const HuffmanCode& huffman_code) : huffman_code_(huffman_code) {}

void Compressor::Encode(istream& in, ostream& out) const {
  CachedBitWriter bit_writer(out);
  char c;
  while (in.get(c)) {
    const auto& symbol_info = huffman_code_.GetSymbolCompressionInfo(c);
    if (!bit_writer.CanWriteBits(symbol_info.length)) {
      bit_writer.Flush();
    }
    bit_writer.WriteNLastBits(symbol_info.code, symbol_info.length);
  }
  bit_writer.Flush();
}

void Compressor::Decode(istream& in, ostream& out) const {
  CachedBitReader bit_reader(in);
  uint code = 0;
  while (bit_reader.HasBits()) {
    code |= bit_reader.ReadBit();
    auto sym_ptr = huffman_code_.FindSymbolByCode(code);
    if (sym_ptr) {
      out << *sym_ptr;
    } else {
      code = code << 1;
    }

  }
}
