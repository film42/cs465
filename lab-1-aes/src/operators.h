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

  static std::string ff_add(std::string a, std::string b) {

    for(int i = 0; i < a.length(); ++i) {
      a[i] = a[i] ^ b[i];
    }

    return a;
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

    // Set the product to 0
    byte_t product = 0x00;

    // Iterate through each bit
    for(int i = 0; i < 8; ++i) {

      // Check for odd bit
      if( (y & 0x01) == 0x01 ) {
        product = ff_add(product, x);
      }

      // Call to x_time
      x = x_time(x);

      // slide y for next iteration
      y >>= 1;
    }

    // Return the product
    return product;
  }

}

#endif
