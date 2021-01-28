#include "cached_bit_writer.h"

#include "utils.h"

using namespace std;

const size_t CachedBitWriter::MAX_CACHED_BYTES = 10;

CachedBitWriter::CachedBitWriter(ostream& out) : out_(out), bucket_idx(0), bucket_bits_allocated(0) {
  buffer.resize(MAX_CACHED_BYTES);
}

void CachedBitWriter::WriteNLastBits(uint code, uint n) {
  while (n) {
    u_char byte_patch = (code << bucket_bits_allocated) & 0xFF;
    buffer[bucket_idx] |= byte_patch;
    u_char bits_collected = 8 - bucket_bits_allocated;
    code = code >> bits_collected;
    if (bits_collected <= n) {
      n -= bits_collected;
      bucket_bits_allocated = 0;
      if (++bucket_idx == MAX_CACHED_BYTES) {
        // Problem here, sometimes it fucks up
        FlushBuffer();
      }
    } else {
      bucket_bits_allocated += n;
      n = 0;
    }
  }
}

bool CachedBitWriter::CanWriteBits(uint length) const {
  return BufferedBytesCount() * 8 + length < MAX_CACHED_BYTES * 8;
}

void CachedBitWriter::FlushBuffer() {
  WritePrimitive<u_char>(out_, buffer.data(), bucket_idx);
  bucket_idx = 0;
  bucket_bits_allocated = 0;
}

void CachedBitWriter::Flush() {
  if (bucket_bits_allocated) {
    WriteNLastBits(0, 8 - bucket_bits_allocated);
  }
  FlushBuffer();
}

size_t CachedBitWriter::BufferedBytesCount() const { return bucket_idx + (bucket_bits_allocated != 0); }

const std::vector<u_char>& CachedBitWriter::GetBuffer() const { return buffer; }