/**
 * Copyright (c) 2012 MIT License by 6.172 Staff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

// Implements the ADT specified in bitarray.h as a packed array of bits; a bit
// array containing bit_sz bits will consume roughly bit_sz/8 bytes of
// memory.


#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

#include <sys/types.h>
#include <stdint.h>

#include "./bitarray.h"

#define WORDSIZE __WORDSIZE

// ********************************* Types **********************************

// Concrete data type representing an array of bits.
struct bitarray {
  // The number of bits represented by this bit array.
  // Need not be divisible by 8.
  size_t bit_sz;

  // The underlying memory buffer that stores the bits in
  // packed form (8 per byte).
  char *buf;
};


// ******************** Prototypes for static functions *********************

// Rotates a subarray left by an arbitrary number of bits.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
// bit_left_amount is the number of places to rotate the
//                    subarray left
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left(bitarray_t *const bitarray,
                                 const size_t bit_offset,
                                 const size_t bit_length,
                                 const size_t bit_left_amount);

// Rotates a subarray left by one bit.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
static void bitarray_rotate_left_one(bitarray_t *const bitarray,
                                     const size_t bit_offset,
                                     const size_t bit_length);

// Portable modulo operation that supports negative dividends.
//
// Many programming languages define modulo in a manner incompatible with its
// widely-accepted mathematical definition.
// http://stackoverflow.com/questions/1907565/c-python-different-behaviour-of-the-modulo-operation
// provides details; in particular, C's modulo
// operator (which the standard calls a "remainder" operator) yields a result
// signed identically to the dividend e.g., -1 % 10 yields -1.
// This is obviously unacceptable for a function which returns size_t, so we
// define our own.
//
// n is the dividend and m is the divisor
//
// Returns a positive integer r = n (mod m), in the range
// 0 <= r < m.
static size_t modulo(const ssize_t n, const size_t m);

// Produces a mask which, when ANDed with a byte, retains only the
// bit_index th byte.
//
// Example: bitmask(5) produces the byte 0b00100000.
//
// (Note that here the index is counted from right
// to left, which is different from how we represent bitarrays in the
// tests.  This function is only used by bitarray_get and bitarray_set,
// however, so as long as you always use bitarray_get and bitarray_set
// to access bits in your bitarray, this reverse representation should
// not matter.
static char bitmask(const size_t bit_index);


// ******************************* Functions ********************************

bitarray_t *bitarray_new(const size_t bit_sz) {
  // Allocate an underlying buffer of ceil(bit_sz/8) bytes.
  char *const buf = calloc(1, bit_sz / 8 + ((bit_sz % 8 == 0) ? 0 : 1));
  if (buf == NULL) {
    return NULL;
  }

  // Allocate space for the struct.
  bitarray_t *const bitarray = malloc(sizeof(struct bitarray));
  if (bitarray == NULL) {
    free(buf);
    return NULL;
  }

  bitarray->buf = buf;
  bitarray->bit_sz = bit_sz;
  return bitarray;
}

void bitarray_free(bitarray_t *const bitarray) {
  if (bitarray == NULL) {
    return;
  }
  free(bitarray->buf);
  bitarray->buf = NULL;
  free(bitarray);
}

size_t bitarray_get_bit_sz(const bitarray_t *const bitarray) {
  return bitarray->bit_sz;
}

bool bitarray_get(const bitarray_t *const bitarray, const size_t bit_index) {
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to get the nth
  // bit, we want to look at the (n mod 8)th bit of the (floor(n/8)th)
  // byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to produce either a zero byte (if the bit was 0) or a nonzero byte
  // (if it wasn't).  Finally, we convert that to a boolean.
  return (bitarray->buf[bit_index / 8] & bitmask(bit_index)) ?
             true : false;
}

void bitarray_set(bitarray_t *const bitarray,
                  const size_t bit_index,
                  const bool value) {
  assert(bit_index < bitarray->bit_sz);

  // We're storing bits in packed form, 8 per byte.  So to set the nth
  // bit, we want to set the (n mod 8)th bit of the (floor(n/8)th) byte.
  //
  // In C, integer division is floored explicitly, so we can just do it to
  // get the byte; we then bitwise-and the byte with an appropriate mask
  // to clear out the bit we're about to set.  We bitwise-or the result
  // with a byte that has either a 1 or a 0 in the correct place.
  bitarray->buf[bit_index / 8] =
      (bitarray->buf[bit_index / 8] & ~bitmask(bit_index)) |
           (value ? bitmask(bit_index) : 0);
}
/*
 * caller has to only pass bit_index on byte/char boundaries. bit_index+WORDSIZE cannot go past end of array
 */
uint64_t bitarray_get_word(const bitarray_t *const bitarray, const size_t bit_index) {
  assert(bit_index <= bitarray->bit_sz);
  assert(bit_index+WORDSIZE <= bitarray->bit_sz);

  uint64_t res = *((uint64_t*) &(bitarray->buf[bit_index / 8])); //sizeof(char) * (bit_index / 8)
  // Potentiall skip these if bit_index % 8 = 0
  res >>= bit_index % 8;
  res |= (((uint64_t)((unsigned char)(bitarray->buf[(bit_index/8)+8])) & 0xFF) << (56 + (8 - bit_index % 8)) & -(bit_index % 8));
  return res;
}

void bitarray_set_word(bitarray_t *const bitarray,
                  const size_t bit_index,
                  const uint64_t value) {
  uint64_t new_val = value << bit_index % 8;
  uint64_t old_bit = ((unsigned char)(bitarray->buf[bit_index / 8]) << (8 - bit_index % 8)) & 0xFF;
  old_bit >>= (8 - bit_index % 8);
  new_val |= old_bit;

  *((uint64_t*) &(bitarray->buf[bit_index / 8])) = new_val;

  unsigned char high_bit = bitarray->buf[(bit_index/8)+8];
  high_bit >>= bit_index % 8;
  high_bit <<= bit_index % 8;
  /*if((bit_index % 8) != 0)*/
    bitarray->buf[(bit_index/8)+8] = high_bit | ((value >> (64 - bit_index % 8)) & -(bit_index % 8));
}

void bitarray_rotate_slow(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount) {
  assert(bit_offset + bit_length <= bitarray->bit_sz);

  if (bit_length == 0) {
    return;
  }

  // Convert a rotate left or right to a left rotate only, and eliminate
  // multiple full rotations.
  bitarray_rotate_left(bitarray, bit_offset, bit_length,
           modulo(-bit_right_amount, bit_length));
}

static void bitarray_rotate_left(bitarray_t *const bitarray,
                                 const size_t bit_offset,
                                 const size_t bit_length,
                                 const size_t bit_left_amount) {
  for (size_t i = 0; i < bit_left_amount; i++) {
    bitarray_rotate_left_one(bitarray, bit_offset, bit_length);
  }
}

static void bitarray_rotate_left_one(bitarray_t *const bitarray,
                                     const size_t bit_offset,
                                     const size_t bit_length) {
  // Grab the first bit in the range, shift everything left by one, and
  // then stick the first bit at the end.
  const bool first_bit = bitarray_get(bitarray, bit_offset);
  size_t i;
  for (i = bit_offset; i + 1 < bit_offset + bit_length; i++) {
    bitarray_set(bitarray, i, bitarray_get(bitarray, i + 1));
  }
  bitarray_set(bitarray, i, first_bit);
}

static size_t modulo(const ssize_t n, const size_t m) {
  const ssize_t signed_m = (ssize_t)m;
  assert(signed_m > 0);
  const ssize_t result = ((n % signed_m) + signed_m) % signed_m;
  assert(result >= 0);
  return (size_t)result;
}

static char bitmask(const size_t bit_index) {
  return 1 << (bit_index % 8);
}

// -----------

void bitarray_swap(bitarray_t *const bitarray, const size_t index1, const size_t index2) {
  bool value1 = bitarray_get(bitarray, index1);
  bitarray_set(bitarray, index1, bitarray_get(bitarray, index2));
  bitarray_set(bitarray, index2, value1);
}

void bitarray_reverse(bitarray_t *const bitarray,
                      const size_t bit_offset,
                      const size_t bit_length) {
  bitarray_reverse_fast(bitarray, bit_offset, bit_length);
}

void bitarray_reverse_slow(bitarray_t *const bitarray,
                      const size_t bit_offset,
                      const size_t bit_length) {
  int i = bit_length / 2;
  while (i--) {
     bitarray_swap(bitarray, bit_offset + i, bit_offset + bit_length - i - 1);
  }
}

/*void bitarray_reverse_norecurse(bitarray_t *const bitarray,*/
                      /*size_t bit_offset,*/
                      /*size_t bit_length) {*/
  /*while(bit_length >= 2*WORDSIZE){*/
    /*uint64_t beg = bitarray_get_word(bitarray, bit_offset);*/
    /*uint64_t end = bitarray_get_word(bitarray, bit_offset + bit_length - WORDSIZE);*/

    /*bitarray_set_word(bitarray, bit_offset, reverse_lookup(end));*/
    /*bitarray_set_word(bitarray, bit_offset + bit_length - WORDSIZE, reverse_lookup(beg));*/

    /*bit_offset += WORDSIZE;*/
    /*bit_length -= 2*WORDSIZE;*/
  /*}*/
  /*bitarray_reverse_slow(bitarray, bit_offset, bit_length);*/
/*}*/


void bitarray_reverse_fast(bitarray_t *const bitarray,
                      const size_t bit_offset,
                      const size_t bit_length) {
  if(bit_length < 2*WORDSIZE) { // enough on ends
    bitarray_reverse_slow(bitarray, bit_offset, bit_length);
  } else {
    uint64_t beg = bitarray_get_word(bitarray, bit_offset);
    uint64_t end = bitarray_get_word(bitarray, bit_offset + bit_length - WORDSIZE);

    bitarray_set_word(bitarray, bit_offset, reverse_lookup(end));
    bitarray_set_word(bitarray, bit_offset + bit_length - WORDSIZE, reverse_lookup(beg));

    bitarray_reverse_fast(bitarray, bit_offset + WORDSIZE, bit_length - 2*WORDSIZE);
  }
}

void bitarray_rotate_fast(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount) {
  ssize_t divide = modulo(-bit_right_amount, bit_length) + bit_offset;
  assert(divide >= 0);

  assert(bit_offset <= divide);
  assert(divide <= bit_offset + bit_length - 1);

  bitarray_reverse(bitarray, bit_offset, divide - bit_offset);
  bitarray_reverse(bitarray, divide, bit_offset + bit_length - divide);
  bitarray_reverse(bitarray, bit_offset, bit_length);
}

void bitarray_rotate(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount) {
	if (bit_length == 0) return;
  bitarray_rotate_fast(bitarray, bit_offset, bit_length, bit_right_amount);
}

const unsigned char BitReverseTable256[256] = {
  #   define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
  #   define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
  #   define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
      R6(0), R6(2), R6(1), R6(3)
};

uint64_t reverse_lookup (uint64_t to_reverse) {
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

/*uint64_t reverse_lookup_fast(uint64_t to_reverse) {*/
  /*uint64_t v = to_reverse;*/

  /*v = ((v >> 1) & 0x5555555555555555) | ((v & 0x5555555555555555) << 1);*/
  /*v = ((v >> 2) & 0x3333333333333333) | ((v & 0x3333333333333333) << 2);*/
  /*v = ((v >> 4) & 0x0F0F0F0F0F0F0F0F) | ((v & 0x0F0F0F0F0F0F0F0F) << 4);*/
  /*v = ((v >> 8) & 0x00FF00FF00FF00FF) | ((v & 0x00FF00FF00FF00FF) << 8);*/
  /*v = ((v >> 16) & 0x0000FFFF0000FFFF) | ((v & 0x0000FFFF0000FFFF) << 16);*/
  /*[>v = ((v >> 32) & 0x00000000FFFFFFFF) | ((v & 0x00000000FFFFFFFF) << 32);<]*/
  /*v = (v >> 32) | (v << 32);*/
  /*return v;*/
/*}*/
