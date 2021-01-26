#include "bit_stream.h"

#include "utils.h"

using namespace std;

const size_t BitStream::MAX_CACHED_BYTES = 10;

BitStream::BitStream(ostream& out) : output_(out), bucket_idx(0), bucket_bits_allocated(0) {
  buffer.resize(MAX_CACHED_BYTES);
}

void BitStream::WriteNLastBits(uint code, uint n) {
  while (n) {
    u_char byte_patch = (code << bucket_bits_allocated) & 0xFF;
    buffer[bucket_idx] |= byte_patch;
    u_char bits_collected = 8 - bucket_bits_allocated;
    code = code >> bits_collected;
    if (bits_collected <= n) {
      n -= bits_collected;
      bucket_bits_allocated = 0;
      bucket_idx++;
    } else {
      bucket_bits_allocated += n;
      n = 0;
    }
  }
}

bool BitStream::CanWriteBits(uint length) const { return BufferedBytesCount() + length < MAX_CACHED_BYTES * 8; }

void BitStream::flush() {
  if (bucket_bits_allocated) {
    WriteNLastBits(0, 8 - bucket_bits_allocated);
  }
  WritePrimitive<u_char>(output_, buffer.data(), bucket_idx);
  bucket_idx = 0;
  bucket_bits_allocated = 0;
}

size_t BitStream::BufferedBytesCount() const { return bucket_idx + (bucket_bits_allocated != 0); }