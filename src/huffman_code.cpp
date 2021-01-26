#include "huffman_code.h"

using namespace std;

HuffmanCode::HuffmanCode(const HuffmanTree&) {}
const HuffmanCode::SymbolInfo& HuffmanCode::GetSymbolCompressionInfo(char) const {
  static HuffmanCode::SymbolInfo info;
  return info;
}
void HuffmanCode::SerializeTo(ostream&) const {}