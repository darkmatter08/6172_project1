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

#ifndef BITARRAY_H
#define BITARRAY_H

#include <sys/types.h>
#include <stdint.h>


// ********************************* Types **********************************

// Abstract data type representing an array of bits.
typedef struct bitarray bitarray_t;


// ******************************* Prototypes *******************************

// Allocates space for a new bit array.
// bit_sz is the number of bits storable in the resultant bit array
bitarray_t *bitarray_new(const size_t bit_sz);

// Frees a bit array allocated by bitarray_new.
void bitarray_free(bitarray_t *const bitarray);

// Returns the number of bits stored in a bit array.
// Note the invariant bitarray_get_bit_sz(bitarray_new(n)) = n.
size_t bitarray_get_bit_sz(const bitarray_t *const bitarray);

// Indexes into a bit array, retreiving the bit at the specified zero-based
// index.
bool bitarray_get(const bitarray_t *const bitarray, const size_t bit_index);

uint64_t bitarray_get_word(const bitarray_t *const bitarray, const size_t bit_index);

// Indexes into a bit array, setting the bit at the specified zero-based index.
void bitarray_set(bitarray_t *const bitarray,
                  const size_t bit_index,
                  const bool value);

void bitarray_set_word(bitarray_t *const bitarray,
                  const size_t bit_index,
                  const uint64_t value);

// Swaps the values as the given indeces in the bitarray
void bitarray_swap(bitarray_t *const bitarray, const size_t index1, const size_t index2);

// Reverses a subarray
void bitarray_reverse(bitarray_t *const birarry,
                      const size_t bit_offset,
                      const size_t bit_length);

void bitarray_reverse_slow(bitarray_t *const bitarray,
                      const size_t bit_offset,
                      const size_t bit_length);

void bitarray_reverse_fast(bitarray_t *const bitarray,
                      const size_t bit_offset,
                      const size_t bit_length);

//void bitarray_reverse_norecurse(bitarray_t *const bitarray,
                      //size_t bit_offset,
                      //size_t bit_length);
// Rotates a subarray.
//
// bit_offset is the index of the start of the subarray
// bit_length is the length of the subarray, in bits
// bit_right_amount is the number of places to rotate the subarray right
//
// The subarray spans the half-open interval
// [bit_offset, bit_offset + bit_length)
// That is, the start is inclusive, but the end is exclusive.
//
// Note: bit_right_amount can be negative, in which case a left rotation is
// performed.
//
// Example:
// Let ba be a bit array containing the byte 0b10010110; then,
// bitarray_rotate(ba, 0, bitarray_get_bit_sz(ba), -1)
// left-rotates the entire bit array in place.  After the rotation, ba
// contains the byte 0b00101101.
//
// Example:
// Let ba be a bit array containing the byte 0b10010110; then,
// bitarray_rotate(ba, 2, 5, 2) rotates the third through seventh
// (inclusive) bits right two places.  After the rotation, ba contains the
// byte 0b10110100.
void bitarray_rotate_slow(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount);

void bitarray_rotate_fast(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount);

void bitarray_rotate(bitarray_t *const bitarray,
                     const size_t bit_offset,
                     const size_t bit_length,
                     const ssize_t bit_right_amount);

uint64_t reverse_lookup (uint64_t to_reverse);
uint64_t reverse_lookup_fast (uint64_t to_reverse);

#endif  // BITARRAY_H
