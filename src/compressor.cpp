#include "compressor.h"

#include "bit_stream.h"

using namespace std;

Compressor::Compressor(const HuffmanCode& huffman_code) : huffman_code_(huffman_code) {}

void Compressor::Encode(istream& input, ostream& output) const {
  huffman_code_.SerializeTo(output);
  output.put(0);
  char c;
  BitStream bit_stream(output);
  while (input.get(c)) {
    const auto& symbol_info = huffman_code_.GetSymbolCompressionInfo(c);
    if (!bit_stream.CanWriteBits(symbol_info.length)) {
      bit_stream.Flush();
    }
    bit_stream.WriteNLastBits(symbol_info.code, symbol_info.length);
  }
  bit_stream.Flush();
}
