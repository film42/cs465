#ifndef __lab_1_aes_tests__aes__
#define __lab_1_aes_tests__aes__

#include <string>
#include <sstream>
#include <iostream>

#include "box.h"
#include "operators.h"

namespace {

  std::string mix_column(std::string column) {

    std::string r = column;

    r[0] = ff_add( (ff_multiply(0x02, column[0])), (ff_multiply(0x03, column[1])), column[2], column[3] );
    r[1] = ff_add( column[0], (ff_multiply(0x02, column[1])), (ff_multiply(0x03, column[2])), column[3] );
    r[2] = ff_add( column[0], column[1], (ff_multiply(0x02, column[2])), (ff_multiply(0x03, column[3])) );
    r[3] = ff_add( (ff_multiply(0x03, column[0])), column[1], column[2], (ff_multiply(0x02, column[3])) );

    return r;
  }

  std::string inverse_mix_column(std::string column) {

    std::string r = column;

    r[0] = ff_add( (ff_multiply(0x0E, column[0])), (ff_multiply(0x0B, column[1])), (ff_multiply(0x0D, column[2])), (ff_multiply(0x09, column[3])) );
    r[1] = ff_add( (ff_multiply(0x09, column[0])), (ff_multiply(0x0E, column[1])), (ff_multiply(0x0B, column[2])), (ff_multiply(0x0D, column[3])) );
    r[2] = ff_add( (ff_multiply(0x0D, column[0])), (ff_multiply(0x09, column[1])), (ff_multiply(0x0E, column[2])), (ff_multiply(0x0B, column[3])) );
    r[3] = ff_add( (ff_multiply(0x0B, column[0])), (ff_multiply(0x0D, column[1])), (ff_multiply(0x09, column[2])), (ff_multiply(0x0E, column[3])) );

    return r;
  }

  std::string mix_columns(std::string state) {

    auto col_1 = mix_column( state.substr(0,4) );
    auto col_2 = mix_column( state.substr(4,4) );
    auto col_3 = mix_column( state.substr(8,4) );
    auto col_4 = mix_column( state.substr(12)  );

    return col_1 + col_2 + col_3 + col_4;
  }

  std::string inverse_mix_columns(std::string state) {

    auto col_1 = inverse_mix_column( state.substr(0,4) );
    auto col_2 = inverse_mix_column( state.substr(4,4) );
    auto col_3 = inverse_mix_column( state.substr(8,4) );
    auto col_4 = inverse_mix_column( state.substr(12)  );

    return col_1 + col_2 + col_3 + col_4;
  }

  std::string shift_rows(std::string state) {
    std::ostringstream ostream;

    ostream << state[0] << state[4] << state[8] << state[12];
    ostream << state[5] << state[9] << state[13] << state[1];
    ostream << state[10] << state[14] << state[2] << state[6];
    ostream << state[15] << state[3] << state[7] << state[11];

    return ostream.str();
  }

  std::string substitute_bytes(std::string state) {
    
    std::string converted;
    converted.reserve( state.size() );
    
    for( byte_t c : state ) {
      // Convert
      auto ciphered_char = sbox_get(c);
      // Add the converted byte
      converted.push_back( ciphered_char );
    }
    
    return converted;
  }
  
  std::string inverse_substitute_bytes(std::string state) {
    
    std::string converted;
    converted.reserve( state.size() );
    
    for( byte_t c : state ) {
      // Undo conversion
      auto ciphered_char = inverse_sbox_get(c);
      // Add the original byte
      converted.push_back( ciphered_char );
    }
    
    return converted;
  }

}

#endif /* defined(__lab_1_aes_tests__aes__) */
