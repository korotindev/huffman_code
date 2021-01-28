#pragma once
#include <optional>

#include "huffman_tree.h"

class HuffmanCode {
 public:
  struct SymbolInfo {
    uint code;
    uint length;
    bool operator==(const SymbolInfo& other) const;
  };

  struct CodeInfo {
    uint length;
    char sym;
  };

  explicit HuffmanCode(const HuffmanTree& tree);
  const SymbolInfo& GetSymbolCompressionInfo(char sym) const;
  const char* FindSymbolByCode(SymbolInfo info) const;
  void SerializeTo(std::ostream& out) const;
  static HuffmanCode DeserializeFrom(std::istream& in);

 private:
  HuffmanCode() = default;
  void BuildSymbolInfos(std::vector<CodeInfo> info);

  struct SymbolInfoHasher {
    const static std::hash<uint> hasher;
    size_t operator()(SymbolInfo info) const;
  };

  std::unordered_map<char, SymbolInfo> symbol_infos_;
  std::unordered_map<SymbolInfo, char, SymbolInfoHasher> inverted_symbol_infos_;
};