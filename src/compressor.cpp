#include "compressor.h"

#include "bit_stream.h"

using namespace std;

Compressor::Compressor(const HuffmanCode& huffman_code) : huffman_code_(huffman_code) {}

void Compressor::Compress(istream& input, ostream& output) const {
  huffman_code_.SerializeTo(output);
  output.put(0);
  char c;
  BitStream bit_stream(output);
  while (input.get(c)) {
    const auto& symbol_info = huffman_code_.GetSymbolCompressionInfo(c);
    if (!bit_stream.can_write_n_bits(symbol_info.length)) {
      bit_stream.flush();
    }
    bit_stream.write_n_last_bits(symbol_info.code, symbol_info.length);
  }
  bit_stream.flush();
}