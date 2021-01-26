#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class HuffmanTree {
 public:
  struct Node {
    using Ptr = std::unique_ptr<Node>;
    Ptr left;
    Ptr right;
    unsigned long long count;
    char sym;
  };
  using CharStat = std::unordered_map<char, unsigned long long>;

  explicit HuffmanTree(const CharStat& stat);
  const Node::Ptr& GetRoot() const;

 private:
  Node::Ptr root_;
};

HuffmanTree BuildHuffmanTreeFrom(std::istream& in);