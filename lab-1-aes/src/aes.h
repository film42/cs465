#ifndef __lab_1_aes_tests__aes__
#define __lab_1_aes_tests__aes__

#include <string>
#include <sstream>
#include <iostream>

#include "box.h"
#include "operators.h"
#include "constants.h"
#include "utils.h"

namespace {

  //
  // Mix Columns
  //
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

  //
  // Shift Rows
  //
  std::string shift_rows(std::string state) {
    std::ostringstream ostream;

    ostream << state[0] << state[4] << state[8] << state[12];
    ostream << state[5] << state[9] << state[13] << state[1];
    ostream << state[10] << state[14] << state[2] << state[6];
    ostream << state[15] << state[3] << state[7] << state[11];

    return ostream.str();
  }

  //
  // Substitute Bytes
  //
  std::string substitute_bytes(std::string input) {
    
    std::string converted;
    converted.reserve( input.size() );
    
    for( byte_t c : input ) {
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
    
    for( byte_t c : input ) {
      // Undo conversion
      auto ciphered_char = inverse_sbox_get(c);
      // Add the original byte
      converted.push_back( ciphered_char );
    }
    
    return converted;
  }

  //
  // Round Key
  //
  std::string add_round_key(std::string state, std::string round_key) {
  
    std::cout << state.length() << std::endl;
    std::cout << round_key << std::endl;

    for(int i = 0; i < state.size(); ++i) {
      state[i] = ff_add( state[i], round_key[i] );
    }

    return state;
  }

  std::string inverse_add_round_key(std::string state, std::string round_key) {
    return add_round_key(state, round_key);
  }

  //
  // Rotate Word
  //
  // RotWord() takes a word [a0,a1,a2,a3] as input, performs a cyclic permutation, and returns the
  // word [a1,a2,a3,a0].
  std::string rotate_word(std::string word) {
    std::ostringstream ostream;

    ostream << word[1] << word[2] << word[3] << word[0];

    return ostream.str();
  }

  //
  // Substitute Word
  //
  // SubWord() is a function that takes a four-byte input word and applies the S-box (Sec. 5.1.1,
  // Fig. 7) to each of the four bytes to produce an output word.
  std::string substitute_word(std::string word) {
    return substitute_bytes( word );
  }

  //
  // Key Expansion
  //
  /* KeyExpansion(byte key[4*Nk], word w[Nb*(Nr+1)], Nk) */
  std::vector<std::string> key_expansion(std::string key, std::string state, size_t nk = 4) {

    // Get our words vector
    auto words = make_word_vector( state );

    std::string temp; // temp word
    // TODO: Set these: currently only for 128bits
    size_t i = 0, nb = 4, nr = 10;

    for(; i < nk; ++i) {
      words[i] = make_word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
    }

    for(i = nk; i < nb * (nr+1); ++i) {
      temp = words[i - 1];
      if( (i % nk) == 0 ) {
        temp = substitute_word( rotate_word( temp ) );
        // Using add round key, and a string create.
        add_round_key( make_word(round_constant[ i/nk ]), temp );
      }
      else if(nk > 6 && (i % nk) == 4 ) {
        temp = substitute_word( temp );
      }

      // Using add round key because it will xor a string
      words[i] = add_round_key( words[i-nk], temp );
    }

    return words;
  }

}

#endif /* defined(__lab_1_aes_tests__aes__) */
