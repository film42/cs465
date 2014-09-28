#include <iostream>
#include <sstream>

#include "test_helper.h"
#include "sha1.h"

TEST(MacAttackTests, playground) {

  SHA1 sha1;

  sha1.set_iv("f4b645e89faaec2ff8e443c595009c16dbdfba4b");

  std::string original = std::string( message_1.begin(), message_1.end() );
  std::string extension = "P. S. Give Garrett 100%";
  uint64_t message_2_bit_size = extension.length() * 8;
  uint64_t message_1_bit_size = original.length() * 8;

  EXPECT_EQ( 376, message_1_bit_size );

  uint64_t key_length = 128;
  uint64_t length_size = 64;

  uint64_t message_1_padding = 1024 - (message_1_bit_size + key_length + length_size);
  uint64_t message_1_full_length = key_length + message_1_bit_size + message_1_padding + length_size; // 376 + 128 + 456 + 64
  uint64_t length = message_1_full_length + message_2_bit_size;

  sha1.update( extension );

  std::cout << "Padding: " << message_1_padding << std::endl;

  EXPECT_EQ( 456 , padding(message_1_padding).length() );
  EXPECT_EQ( '1' , padding(message_1_padding)[0] );
  EXPECT_EQ( '0' , padding(message_1_padding)[1] );

  std::ostringstream sever_message_base;
  sever_message_base
      << string_to_binary_string(original)
      << padding(message_1_padding + key_length + message_1_bit_size )
      // I think this needs to be the ( original message || key ) in bits for size:
      << gen_length( message_1_bit_size + key_length );

  std::ostringstream server_message;
  server_message
      << sever_message_base.str()
      << string_to_binary_string(extension);


  std::cout << "Using hacked length: " << length << std::endl;
  std::string digest = sha1.final( length );

  std::cout << "Hex: " << binary_string_to_hex_string( server_message.str() ) << std::endl;

  std::cout << digest << std::endl;

  EXPECT_EQ( 456, message_1_padding );
  EXPECT_EQ( 64, gen_length(123).length() );
  EXPECT_EQ( message_1_padding, padding(message_1_padding).length() );
}

TEST(MacAttackTests, sha1_works) {

  SHA1 sha1;
  sha1.update("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc");

  EXPECT_EQ( "284f2b4b1a934e36ef357c41b84cb28492b6696d" , sha1.final() );
}