#pragma once
#include "huffman_tree.h"

class HuffmanCode {
 public:
  struct SymbolInfo {
    uint code;
    uint length;
  };

  struct CodeInfo {
    uint length;
    char sym;
  };

  explicit HuffmanCode(const HuffmanTree& tree);
  const SymbolInfo& GetSymbolCompressionInfo(char sym) const;
  void SerializeTo(std::ostream& out) const;
  static HuffmanCode DeserializeFrom(std::istream& in);

 private:
  HuffmanCode() = default;
  void BuildSymbolInfos(std::vector<CodeInfo> info);

  std::unordered_map<char, SymbolInfo> symbol_infos_;
};