#include "test_helper.h"
#include "aes.h"
#include "utils.h"

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

TEST(AESTests, can_add_round_key_column) {

  char state_col[] = { (char)0x04, (char)0x66, (char)0x81, (char)0xE5, '\0' };

  char round_col[] = { (char)0xA0, (char)0xFA, (char)0xFE, (char)0x17, '\0' };

  char expect_col[] = { (char)0xA4, (char)0x9C, (char)0x7F, (char)0xF2, '\0' };

  auto state = std::string( state_col );
  auto round = std::string( round_col );
  auto after = std::string( expect_col );

  EXPECT_EQ( after, add_round_key(state, round) );
}

TEST(AESTests, can_add_round_key) {

  char state_col[] = { (char)0x58, (char)0x1B, (char)0xDB, (char)0x1B,
                       (char)0x4D, (char)0x4B, (char)0xE7, (char)0x6B,
                       (char)0xCA, (char)0x5A, (char)0xCA, (char)0xB0,
                       (char)0xF1, (char)0xAC, (char)0xA8, (char)0xE5,'\0' };

  char round_col[] = { (char)0xF2, (char)0x7A, (char)0x59, (char)0x73,
                       (char)0xC2, (char)0x96, (char)0x35, (char)0x59,
                       (char)0x95, (char)0xB9, (char)0x80, (char)0xF6,
                       (char)0xF2, (char)0x43, (char)0x7A, (char)0x7F,'\0' };

  char expect_col[] = { (char)0xAA, (char)0x61, (char)0x82, (char)0x68,
                        (char)0x8F, (char)0xDD, (char)0xD2, (char)0x32,
                        (char)0x5F, (char)0xE3, (char)0x4A, (char)0x46,
                        (char)0x03, (char)0xEF, (char)0xD2, (char)0x9A,'\0' };

  auto state = std::string( state_col );
  auto round = std::string( round_col );
  auto after = std::string( expect_col );

  EXPECT_EQ( after, add_round_key(state, round) );
}

TEST(AESTests, can_rotate_word) {

  EXPECT_EQ( "2341", rotate_word("1234") );

}

TEST(AESTests, can_do_key_expansion_in_tests) {

  auto temp = make_word(0x64636261);
  auto after_rotate = make_word(0x63626164);
  auto after_sub_word = make_word(0xfbaaef43);
  auto after_rconn = make_word(0x01000000);
  auto after_xor_rconn = make_word(0xfaaaef43);
  auto w_i_minus_nk = make_word(0x706f6e6d);
  auto after_all = make_word(0x8ac5812e);


  EXPECT_EQ( after_rotate , rotate_word(temp) );
  EXPECT_EQ( after_sub_word , substitute_word(after_rotate) );
  EXPECT_EQ( after_rconn , make_word( round_constant[1] ) );
  EXPECT_EQ( after_xor_rconn , ff_add(after_rconn, after_sub_word) );
  EXPECT_EQ( after_all , ff_add(after_xor_rconn, w_i_minus_nk) );
}

TEST(AESTests, can_do_key_expansion) {

  auto state = "abcdefghijklmnop";
  auto key = "ponmlkjihgfedcba";

  auto expanded_key = key_expansion(key, state);

  EXPECT_EQ( 44 , expanded_key.size() );

  EXPECT_EQ( make_word(0x8ac5812e), expanded_key[4] );
  EXPECT_EQ( make_word(0xa01a65af), expanded_key[14] );
  EXPECT_EQ( make_word(0xe83e0bde), expanded_key[24] );
  EXPECT_EQ( make_word(0x786fb036), expanded_key[43] );
}