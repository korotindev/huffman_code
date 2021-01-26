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
  struct HuffmanCode {
    unsigned long long size;
    unsigned long long code;
  };
  using HuffmanCodes = std::unordered_map<char, HuffmanCode>;

  HuffmanTree(const CharStat& stat);
  const Node::Ptr& GetRoot() const;

 private:
  Node::Ptr root;
};

HuffmanTree BuildHuffmanTreeFrom(std::istream& input);