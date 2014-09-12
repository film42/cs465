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

    r[0] = ff_add( (ff_multiply(0x02, (byte_t)column[0])), (ff_multiply(0x03, (byte_t)column[1])), (byte_t)column[2], (byte_t)column[3] );
    r[1] = ff_add( (byte_t)column[0], (ff_multiply(0x02, (byte_t)column[1])), (ff_multiply(0x03, (byte_t)column[2])), (byte_t)column[3] );
    r[2] = ff_add( (byte_t)column[0], (byte_t)column[1], (ff_multiply(0x02, (byte_t)column[2])), (ff_multiply(0x03, (byte_t)column[3])) );
    r[3] = ff_add( (ff_multiply(0x03, (byte_t)column[0])), (byte_t)column[1], (byte_t)column[2], (ff_multiply(0x02, (byte_t)column[3])) );

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

    ostream << state[0] << state[5] << state[10] << state[15];
    ostream << state[4] << state[9] << state[14] << state[3];
    ostream << state[8] << state[13] << state[2] << state[7];
    ostream << state[12] << state[1] << state[6] << state[11];

    return ostream.str();
  }

  std::string inverse_shift_rows(std::string state) {

    std::ostringstream ostream;

    ostream << state[0] << state[13] << state[10] << state[7];
    ostream << state[4] << state[1] << state[14] << state[11];
    ostream << state[8] << state[5] << state[2] << state[15];
    ostream << state[12] << state[9] << state[6] << state[3];

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
    return ff_add(state, round_key);
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
  std::vector<std::string> key_expansion(std::string key, size_t nk, size_t nr) {

    size_t i = 0, nb = 4;

    // Get our words vector
    size_t size = nb * (nr + 1);
    auto words = make_word_vector( size );

    std::string temp; // temp word

    for(; i < nk; ++i) {
      words[i] = make_word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]);
    }

    for(i = nk; i < nb * (nr+1); ++i) {
      temp = words[i - 1];
      if( (i % nk) == 0 ) {
        temp = substitute_word( rotate_word( temp ) );
        // Using ff_add, and a string create.
        temp = ff_add( make_word(round_constant[ i/nk ]), temp );
      }
      else if(nk > 6 && (i % nk) == 4 ) {
        temp = substitute_word( temp );
      }

      // Using ff_add because it will xor a string
      auto new_word = ff_add( words[i-nk], temp );

//      std::cout << string_to_hex(new_word) << std::endl;

      words[i] = new_word;
    }

    return words;
  }

  std::vector<std::string> key_expansion_128_bit(std::string key) {
    return key_expansion(key, 4, 10);
  }

  std::vector<std::string> key_expansion_192_bit(std::string key) {
    return key_expansion(key, 6, 12);
  }

  std::vector<std::string> key_expansion_256_bit(std::string key) {
    return key_expansion(key, 8, 14);
  }

  //
  // AES Cipher
  //
  std::string cipher(std::string state, std::vector<std::string> key_schedule, size_t nr) {

    size_t i = 0, nb = 4;

    state = add_round_key( state, partition(key_schedule, 0, (nb-1) ) );

    // Loop through each round
    for(int round = 1; round <= (nr - 1); ++round) {
      state = substitute_bytes( state );
      state = shift_rows( state );
      state = mix_columns( state );
      state = add_round_key( state, partition(key_schedule, (round * nb), ((round+1) * nb-1) ) );
    }

    // Last thing
    state = substitute_bytes( state );
    state = shift_rows( state );
    state = add_round_key( state, partition(key_schedule, (nr * nb), ((nr+1) * nb-1) ) );

    return state;
  }

  std::string cipher_128_bit(std::string state, std::vector<std::string> key_schedule) {
    return cipher(state, key_schedule, 10);
  }

  std::string cipher_192_bit(std::string state, std::vector<std::string> key_schedule) {
    return cipher(state, key_schedule, 12);
  }

  std::string cipher_256_bit(std::string state, std::vector<std::string> key_schedule) {
    return cipher(state, key_schedule, 14);
  }

  //
  // AES Inverse Cipher
  //
  std::string inverse_cipher(std::string state, std::vector<std::string> key_schedule, size_t nr) {

    size_t i = 0, nb = 4;

    state = inverse_add_round_key(state, partition(key_schedule, (nr*nb), ((nr+1) * nb-1) ) );

    for(size_t round = (nr - 1); round >= 1; --round) {
      state = inverse_shift_rows(state);
      state = inverse_substitute_bytes(state);
      state = inverse_add_round_key(state, partition(key_schedule, (round*nb), ((round+1) * nb-1) ) );
      state = inverse_mix_columns(state);
    }

    state = inverse_shift_rows(state);
    state = inverse_substitute_bytes(state);
    state = add_round_key(state, partition(key_schedule, 0, (nb-1) ) );

    return state;
  }

  std::string inverse_cipher_128_bit(std::string state, std::vector<std::string> key_schedule) {
    return inverse_cipher(state, key_schedule, 10);
  }

  std::string inverse_cipher_192_bit(std::string state, std::vector<std::string> key_schedule) {
    return inverse_cipher(state, key_schedule, 12);
  }

  std::string inverse_cipher_256_bit(std::string state, std::vector<std::string> key_schedule) {
    return inverse_cipher(state, key_schedule, 14);
  }

}

#endif /* defined(__lab_1_aes_tests__aes__) */
