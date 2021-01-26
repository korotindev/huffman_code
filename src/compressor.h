#pragma once

#include <iostream>

#include "huffman_code.h"

class Compressor {
  const HuffmanCode& huffman_code_;

 public:
  explicit Compressor(const HuffmanCode& huffman_code);
  void Compress(std::istream& input, std::ostream& output) const;
};