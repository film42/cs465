#include "test_helper.h"
#include "aes.h"

TEST(AESTests, can_convert_string_both_ways) {
  
  std::string state = "testing";
  
  auto ciphered_text = substitute_bytes( state );
  
  EXPECT_EQ( ciphered_text , "\222M\217\222\371\237\205" );
  
  EXPECT_EQ( state , inverse_substitute_bytes(ciphered_text) );
}