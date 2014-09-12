#ifndef __lab_1_aes_tests_utils__
#define __lab_1_aes_tests_utils__

#include <string>
#include <vector>

#include "types.h"

namespace {

  //
  // Make word with 4 chars
  //
  std::string make_word(char a, char b, char c, char d) {

    std::ostringstream ostream;

    ostream << a;
    ostream << b;
    ostream << c;
    ostream << d;

    return ostream.str();
  }

  std::string make_word(int number) {

    std::ostringstream ostream;

    // Start at n-2 = 6
    for(int i = 6; i >= 0; i -= 2) {
      byte_t c = (byte_t)( (number >> (byte_t)(i * 4)) & ((1 << 8)-1) );

      ostream << (unsigned char)c;
    }

    return ostream.str();
  }

  //
  // Make word vector
  //
  std::vector<std::string> make_word_vector(size_t size ) {

    return std::vector<std::string>(size);

  }

  std::string partition(std::vector<std::string> collection, size_t start, size_t end) {

    std::string result;

    for(size_t i = start; i <= end; ++i) {
      result += collection[i];
    }

    return result;
  }

  std::string string_to_hex(std::string input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
      const byte_t c = input[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
    }
    return output;
  }

}

#endif