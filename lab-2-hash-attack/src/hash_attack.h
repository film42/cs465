#ifndef HASH_ATTACK_HASH_ATTACK
#define HASH_ATTACK_HASH_ATTACK

#include <openssl/sha.h>
#include <string>

namespace {

  static std::string tiny_sha1( std::string input ) {

    unsigned char buffer[6];

    memset(buffer, 0, 5);

    SHA1((unsigned const char *)input.c_str(), 5, buffer);

    return std::string( (const char *)buffer, 5 );
  }

  static std::string random_string() {
    // We want chars from 48 - 122
    int string_size = 6;
    std::string result;
    result.reserve(string_size);

    for(int i = 0; i < string_size; ++i ) {
      char random_char = (char)((rand() % 74) + 48);

      result.push_back( random_char );
    }

    return result;
  }

  static std::string random_tiny_sha1() {

    return tiny_sha1( random_string() );
  }

};

#endif