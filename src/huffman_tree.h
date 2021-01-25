#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class HuffmanTree {
 public:
  struct Node;
  using NodePtr = std::unique_ptr<Node>;
  struct Node {
    NodePtr left;
    NodePtr right;
    unsigned long long count;
    char sym;
  };
  using CharStat = std::unordered_map<char, unsigned long long>;
  using SymBitmask = std::unordered_map<char, std::string>;

  HuffmanTree(const CharStat& stat);

  SymBitmask BuildSymBitmask() const;

 private:
  NodePtr root;
};