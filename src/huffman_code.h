#pragma once
#include <optional>

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
  const char* FindSymbolByCode(uint code) const;
  void SerializeTo(std::ostream& out) const;
  static HuffmanCode DeserializeFrom(std::istream& in);

 private:
  HuffmanCode() = default;
  void BuildSymbolInfos(std::vector<CodeInfo> info);

  std::unordered_map<char, SymbolInfo> symbol_infos_;
  std::unordered_map<uint, char> inverted_symbol_infos_;
};