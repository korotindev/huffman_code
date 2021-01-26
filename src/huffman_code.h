#pragma once
#include "huffman_tree.h"

class HuffmanCode {
 public:
  struct SymbolInfo {
    uint code;
    uint length;
  };

  explicit HuffmanCode(const HuffmanTree& tree);
  const SymbolInfo& GetSymbolCompressionInfo(char sym) const;
  void SerializeTo(std::ostream& out) const;

 private:
  std::unordered_map<char, SymbolInfo> symbol_infos_;
};