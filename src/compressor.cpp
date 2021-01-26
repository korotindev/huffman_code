#include "compressor.h"

#include "cached_bit_writer.h"

using namespace std;

Compressor::Compressor(const HuffmanCode& huffman_code) : huffman_code_(huffman_code) {}

void Compressor::Encode(istream& input, ostream& out) const {
  huffman_code_.SerializeTo(out);

  CachedBitWriter bit_writer(out);
  char c;
  while (input.get(c)) {
    const auto& symbol_info = huffman_code_.GetSymbolCompressionInfo(c);
    if (!bit_writer.CanWriteBits(symbol_info.length)) {
      bit_writer.Flush();
    }
    bit_writer.WriteNLastBits(symbol_info.code, symbol_info.length);
  }
  bit_writer.Flush();
}
