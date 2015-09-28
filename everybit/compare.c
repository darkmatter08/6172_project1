#include <stdint.h>

const unsigned char BitReverseTable256[256] = {
  #   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
  #   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
  #   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
      R6(0), R6(2), R6(1), R6(3)
};

uint64_t reverse_lookup (uint64_t v) {
  v = ((v >> 1) & 0x5555555555555555) | ((v & 0x5555555555555555) << 1);
  v = ((v >> 2) & 0x3333333333333333) | ((v & 0x3333333333333333) << 2);
  v = ((v >> 4) & 0x0F0F0F0F0F0F0F0F) | ((v & 0x0F0F0F0F0F0F0F0F) << 4);
  v = ((v >> 8) & 0x00FF00FF00FF00FF) | ((v & 0x00FF00FF00FF00FF) << 8);
  v = ((v >> 16) & 0x0000FFFF0000FFFF) | ((v & 0x0000FFFF0000FFFF) << 16);
  v = (v >> 32) | (v << 32);
  return v;
}

uint64_t reverse_lookup_fast (uint64_t to_reverse) {
  uint64_t reverse =
      ((uint64_t)(BitReverseTable256[to_reverse & 0xff]) << 56) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 8) & 0xff]) << 48) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 16) & 0xff]) << 40) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 24) & 0xff]) << 32) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 32) & 0xff]) << 24) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 40) & 0xff]) << 16) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 48) & 0xff]) << 8) |
      ((uint64_t)(BitReverseTable256[(to_reverse >> 56) & 0xff]));
  return reverse;
}