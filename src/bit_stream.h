#pragma once

#include <iostream>
#include <vector>

class BitStream {
  std::ostream& output_;
  std::vector<u_char> buffer;
  size_t bucket_idx;
  u_char bucket_bits_allocated;

 public:
  static const size_t MAX_CACHED_BYTES;

  explicit BitStream(std::ostream& out);
  void WriteNLastBits(uint code, uint n);
  bool CanWriteBits(uint length) const;
  void Flush();
  size_t BufferedBytesCount() const;
};