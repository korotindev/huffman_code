#include "cached_bit_reader.h"

#include <iostream>
#include <vector>

#include "utils.h"

const size_t CachedBitReader::MAX_CACHED_BYTES = 10;

CachedBitReader::CachedBitReader(std::istream& in)
    : in_(in), buffer(MAX_CACHED_BYTES), bucket_idx(0), bucket_bit_idx(0), buckets_cached(0) {}

bool CachedBitReader::HasBits() const { return in_ || bucket_idx < buckets_cached; }

bool CachedBitReader::ReadBit() {
  if (bucket_bit_idx == 8) {
    bucket_idx++;
    bucket_bit_idx = 0;
  }

  if (bucket_idx == buckets_cached) {
    CacheNextChunk();
    bucket_idx = 0;
    bucket_bit_idx = 0;
  }

  bool bit = buffer[bucket_idx] & 1;
  buffer[bucket_idx] = buffer[bucket_idx] >> 1;
  bucket_bit_idx++;

  return bit;
}

void CachedBitReader::CacheNextChunk() {
  ReadPrimitive(in_, buffer.data(), MAX_CACHED_BYTES);
  buckets_cached = in_.gcount();
}
