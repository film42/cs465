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
    output.reserve(16);

    for(int i = 0; i < 16; ++i) {
      char c = (char)( (number >> (i * 4)) & ((1 << 4)-1) );
      output.push_back( c );
    }

    return output;
  }

  //
  // Make word vector from state string
  //
  std::vector<std::string> make_word_vector( std::string state ) {

    auto col_1 = state.substr(0,4);
    auto col_2 = state.substr(4,4);
    auto col_3 = state.substr(8,4);
    auto col_4 = state.substr(12);

    return { col_1, col_2, col_3, col_4 };
  }

}

#endif