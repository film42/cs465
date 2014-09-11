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

    char _slot[] = { a, b, c, d, '\0' };

    return std::string(_slot);
  }

  std::string make_word(unsigned int number) {

    std::string output;
    output.reserve(8);

    // Start at n-2 = 6
    for(int i = 6; i >= 0; i -= 2) {
      char c = (char)( (number >> (i * 4)) & ((1 << 8)-1) );
      output.push_back( c );
    }

    return output;
  }

  //
  // Make word vector from state string
  //
  std::vector<std::string> make_word_vector( std::string state, size_t size ) {

    std::vector<std::string> output( size );

    output[0] = state.substr(0,4);
    output[1] = state.substr(4,4);
    output[2] = state.substr(8,4);
    output[3] = state.substr(12);

    return output;
  }

}

#endif