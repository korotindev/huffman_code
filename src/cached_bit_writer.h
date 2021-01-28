#pragma once

#include <iostream>
#include <vector>

class CachedBitWriter {
  std::ostream& out_;
  std::vector<u_char> buffer;
  size_t bucket_idx;
  u_char bucket_bits_allocated;

  void FlushBuffer();
 public:
  static const size_t MAX_CACHED_BYTES;

  explicit CachedBitWriter(std::ostream& out);
  void WriteNLastBits(uint code, uint n);
  bool CanWriteBits(uint length) const;
  void Flush();
  size_t BufferedBytesCount() const;
  const std::vector<u_char>& GetBuffer() const;
};