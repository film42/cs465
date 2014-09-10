#include "test_helper.h"
#include "aes.h"

TEST(AESTests, can_convert_string_both_ways) {
  
  std::string state = "testing";
  
  auto ciphered_text = substitute_bytes( state );
  
  EXPECT_EQ( ciphered_text , "\222M\217\222\371\237\205" );
  
  EXPECT_EQ( state , inverse_substitute_bytes(ciphered_text) );
}

TEST(AESTests, can_shift_rows) {

  std::string state = "abcdefghijklmnop";

  std::string shifted_row_state = "aeimfjnbkocgpdhl";

  EXPECT_EQ( shifted_row_state, shift_rows(state) );
}

TEST(AESTests, can_mix_single_column) {

  char state_col[] = { (char)0xd4, (char)0xbf, (char)0x5d, (char)0x30, '\0' };
  std::string state( state_col, 4 );

  char result_col[] = { (char)0x04, (char)0x66, (char)0x81, (char)0xe5, '\0' };
  std::string result( result_col, 4 );

  EXPECT_EQ( result, mix_column(state) );

  EXPECT_EQ( state, inverse_mix_column(result) );

}

TEST(AESTests, can_mix_columns) {

  std::string state = "abcdefghijklmnop";

  std::string mixed_state = mix_columns(state);

  EXPECT_EQ( state, inverse_mix_columns(mixed_state) );

}