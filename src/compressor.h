#pragma once

#include <iostream>

#include "huffman_code.h"

class Compressor {
  const HuffmanCode& huffman_code_;

 public:
  explicit Compressor(const HuffmanCode& huffman_code);
  void Encode(std::istream& in, std::ostream& out) const;
  void Decode(std::istream& in, std::ostream& out) const;
};