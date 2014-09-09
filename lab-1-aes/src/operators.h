#ifndef lab_1_aes_tests_operators_h
#define lab_1_aes_tests_operators_h

#include <cmath>

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
  static byte_t ff_multiply( byte_t x, byte_t y ) {
    byte_t acc = x;
    byte_t i = y;
    while( i > 0x01 ) {
      acc = x_time( acc );
      i >>= 0x01;
    }

    if( (y & 0x01) == 0x01 ) {
      acc = ff_add( acc, x );
      // Decrement the y to note we accounted for the odd bit
      y -= 0x01;
    }

    if( (y - 0x10) > 0 ) {
      // We guarantee to handle up to 0x10 per iteration, recurse if `y` exceeds this
      auto res = ff_multiply( x , (y - 0x10) );
      // Add what was found
      acc = ff_add( acc, res );
    }

    return acc;
  }

}

#endif
