#include "bit_stream.h"

using namespace std;

BitStream::BitStream(ostream& out) : output_(out) {}

void BitStream::write_n_last_bits(uint, uint) {}
bool BitStream::can_write_n_bits(uint) const { return false; }
void BitStream::flush() {}