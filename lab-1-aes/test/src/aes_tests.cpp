#include "test_helper.h"
#include "aes.h"

TEST(AESTests, can_convert_string_both_ways) {
  
  std::string input = "testing";
  
  auto ciphered_text = substitute_bytes( input );
  
  EXPECT_EQ( ciphered_text , "\222M\217\222\371\237\205" );
  
  EXPECT_EQ( input , inverse_substitute_bytes(ciphered_text) );
}