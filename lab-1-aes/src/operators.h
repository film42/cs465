#ifndef lab_1_aes_tests_operators_h
#define lab_1_aes_tests_operators_h

#include "types.h"

namespace {

  static byte_t ff_add( byte_t x, byte_t y ) {
    // addition is just xor
    return x ^ y;
  }

  static byte_t ff_add( byte_t a, byte_t b, byte_t c, byte_t d) {
    return a ^ b ^ c ^ d;
  }

  static byte_t x_time( byte_t x ) {
    // bit-shift once
    int s = (x << 0x01);
    // check for overflow
    if( s > 0xFF ) { s ^= 0x1B; }
    // return
    return (byte_t)s;
  }

  //
  // Finite Field Operators
  //
  // SOURCE: http://www.samiam.org/galois.html
  //
  byte_t ff_multiply(byte_t a, byte_t b) {
    byte_t p = 0;
    byte_t counter;
    byte_t hi_bit_set;
    for(counter = 0; counter < 8; counter++) {
      if((b & 1) == 1)
        p ^= a;
      hi_bit_set = (a & 0x80);
      a <<= 1;
      if(hi_bit_set == 0x80)
        a ^= 0x1b;
      b >>= 1;
    }
    return p;
  }
}

#endif
