#ifndef HASH_ATTACK_HASH_ATTACK
#define HASH_ATTACK_HASH_ATTACK

#include <openssl/sha.h>
#include <string>
#include <bitset>
#include <sstream>

#include "db.h"

namespace {

  static std::string tiny_sha1( int bit_count,  std::string input ) {

    // Buffer of just over 160 bits
    unsigned char buffer[21];

    // Generate SHA1 from the input string
    SHA1((unsigned const char *)input.c_str(), input.length(), buffer);

    // Convert the buffer to a proper string
    std::string hash_string = std::string( (const char *)buffer, 20 );

    // Now concatenate all the bits
    std::stringstream ostream;

    // Append the bits from each byte as a string
    for( char c : hash_string) {
      ostream << std::bitset<8>( c ).to_string();
    }

    // Get our binary stream from the string stream buffer
    std::string binary_string = ostream.str();

    // Return a substring of the bit_count
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

  static void collision_test(int bit_count, int test_size) {
    auto database = Database::get_instance();
    int collision_count = 0;
    uint64_t attempts = 0;
    uint64_t first = 0;


    std::cout << "Bit Count: " << bit_count << std::endl;
    std::cout << "Test Size: " << test_size << std::endl;
    std::cout << "Prediction: " << ( std::pow(2, (bit_count / 2)) ) << std::endl;

    while( collision_count < test_size ) {

      auto a_random_string = random_string();

      ++attempts;


      // If it's not an original string, then don't use it
      if( database->original_exists( a_random_string ) ) {
        continue;
      }


      auto hash = tiny_sha1( bit_count, a_random_string );
      database->insert( a_random_string , hash );

      if( database->collision_for_hash( hash ) ) {
        ++collision_count;

        database->erase();

        // DEBUG
        if( collision_count == 1 ) {
          first = attempts;
        }
      }

    }

    std::cout << "First: " << first << std::endl;
    std::cout << "Average: " << (attempts / test_size) << std::endl;
    std::cout << "Attempts: " << attempts << std::endl;
    std::cout << "Collisions: " << database->collision_count() << std::endl;
  }

  static void pre_image_test(int bit_count,  int test_size, std::string pre_image) {
    auto database = Database::get_instance();

    int collision_count = 0;
    uint64_t attempts = 0;
    uint64_t first = 0;

    std::cout << "Bit Count: " << bit_count << std::endl;
    std::cout << "Test Size: " << test_size << std::endl;
    std::cout << "Prediction: " << ( std::pow(2, bit_count) ) << std::endl;

    std::string official_hash = tiny_sha1( bit_count, pre_image );

    while (collision_count < test_size) {

      auto a_random_string = random_string();


      // If it's not an original string, then don't use it
      if (database->original_exists(a_random_string)) {
        continue;
      }

      ++attempts;

      // This is a pre-image attack, but this ensures that we don't duplicate random strings
      auto hash = tiny_sha1( bit_count, a_random_string );
      database->insert( a_random_string , hash );

      if( hash == official_hash ) {
        ++collision_count;

        // DEBUG
        if( collision_count == 1 ) {
          first = attempts;
        }
      }

    }

    std::cout << "First: " << first << std::endl;
    std::cout << "Average: " << (attempts / test_size) << std::endl;
    std::cout << "Attempts: " << attempts << std::endl;
  }

};

#endif