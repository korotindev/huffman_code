#include "cached_bit_reader.h"

#include <iostream>
#include <vector>

#include "utils.h"

const size_t CachedBitReader::MAX_CACHED_BYTES = 10;

CachedBitReader::CachedBitReader(std::istream& in) : in_(in) { buffer.resize(MAX_CACHED_BYTES); }
bool CachedBitReader::HasBits() const { return false; }
bool CachedBitReader::ReadBit() { return false; }

void CachedBitReader::CacheNextChunk() {
  ReadPrimitive(in_, buffer.data(), MAX_CACHED_BYTES);
  buckets_cached = in_.gcount();
}
