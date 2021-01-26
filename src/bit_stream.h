#pragma once
#include <iostream>

class BitStream {
  std::ostream& output_;

 public:
  BitStream(std::ostream& out);
  void write_n_last_bits(uint packed_bits, uint length);
  bool can_write_n_bits(uint length) const;
  void flush();
};