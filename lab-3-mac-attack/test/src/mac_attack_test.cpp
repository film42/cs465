#include <iostream>

#include "test_helper.h"
#include "sha1.h"

TEST(MacAttackTests, playground) {

  SHA1 sha1;

  sha1.set_iv("f4b645e89faaec2ff8e443c595009c16dbdfba4b");

  std::string original = "No one has completed lab 2 so give them all a 0%";
  std::string extension = "P. S. Give Garrett 100%";
  size_t message_2_bit_size = extension.length() * 8;
  size_t message_1_bit_size = original.size() * 8;
  size_t key_length = 128;
  size_t message_1_padding = 447 - ((message_1_bit_size + key_length) % 447);
  size_t length = key_length + message_1_bit_size + message_1_padding + 64 + message_2_bit_size;

  sha1.update( extension );

  EXPECT_EQ( 382, message_1_padding );
  EXPECT_EQ( 64, gen_length(123).length() );
  EXPECT_EQ( message_1_padding, padding(message_1_padding).length() );

  auto out_string = original + padding(message_1_padding) +
      gen_length(message_1_bit_size + key_length) + extension;

  auto input =  string_to_hex( out_string );

  std::cout << input << std::endl;
  std::cout << out_string << std::endl;

  std::cout << sha1.final( length ) << std::endl;
}

TEST(MacAttackTests, sha1_works) {

  SHA1 sha1;
  sha1.update("abc");

  EXPECT_EQ( "a9993e364706816aba3e25717850c26c9cd0d89d" , sha1.final() );
}