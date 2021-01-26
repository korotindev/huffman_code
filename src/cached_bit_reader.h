#pragma once

#include <iostream>
#include <vector>

class CachedBitReader {
  std::istream& in_;
  std::vector<u_char> buffer;
  size_t bucket_idx;
  u_char bucket_bit_idx;
  size_t buckets_cached;

 public:
  static const size_t MAX_CACHED_BYTES;
  explicit CachedBitReader(std::istream& in);
  bool HasBits() const;
  bool ReadBit();
  void CacheNextChunk();
};