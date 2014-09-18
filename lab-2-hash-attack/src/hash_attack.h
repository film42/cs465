#ifndef HASH_ATTACK_HASH_ATTACK
#define HASH_ATTACK_HASH_ATTACK

#include <openssl/sha.h>
#include <string>
#include <bitset>
#include <sstream>

namespace {

  static std::string tiny_sha1( int bit_count,  std::string input ) {

    unsigned char buffer[21];

    SHA1((unsigned const char *)input.c_str(), input.length(), buffer);

    std::string hash_string = std::string( (const char *)buffer, 20 );

    std::stringstream ostream;

    for( char c : hash_string) {
      ostream << std::bitset<8>( c ).to_string();
    }

    std::string binary_string = ostream.str();

    return binary_string.substr(0, bit_count);
  }

  static std::string random_string() {
    // We want chars from 48 - 122
    int string_size = 16;
    std::string result;
    result.reserve(string_size);

    for(int i = 0; i < string_size; ++i ) {
      char random_char = (char)((rand() % 74) + 48);

      result.push_back( random_char );
    }

    return result;
  }

};

#endif