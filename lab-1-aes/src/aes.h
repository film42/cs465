#ifndef __lab_1_aes_tests__aes__
#define __lab_1_aes_tests__aes__

#include <string>

#include "box.h"

namespace {

  std::string substitute_bytes(std::string input) {
    
    std::string converted;
    converted.reserve( input.size() );
    
    for( char c : input ) {
      // Convert
      auto ciphered_char = sbox_get(c);
      // Add the converted byte
      converted.push_back( ciphered_char );
    }
    
    return converted;
  }
  
  std::string inverse_substitute_bytes(std::string input) {
    
    std::string converted;
    converted.reserve( input.size() );
    
    for( char c : input ) {
      // Undo conversion
      auto ciphered_char = inverse_sbox_get(c);
      // Add the original byte
      converted.push_back( ciphered_char );
    }
    
    return converted;
  }

}

#endif /* defined(__lab_1_aes_tests__aes__) */
