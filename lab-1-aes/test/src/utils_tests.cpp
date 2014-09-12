#include "test_helper.h"
#include "utils.h"

TEST(UtilsTests, can_make_word_from_hex) {

  std::string output = make_word(0x706f6e6d);

  EXPECT_EQ( output , make_word(0x706f6e6d) );
  EXPECT_EQ( 0x70, output[0] );
  EXPECT_EQ( 0x6f, output[1] );
  EXPECT_EQ( 0x6e, output[2] );
  EXPECT_EQ( 0x6d, output[3] );
}

TEST(UtilsTest, can_make_word_from_weird_hex) {

  auto output = make_word(0xDFBF5D30);

  EXPECT_EQ( output , make_word(0xDFBF5D30) );
  EXPECT_EQ( 0xDF, (byte_t)output[0] );
  EXPECT_EQ( 0xBF, (byte_t)output[1] );
  EXPECT_EQ( 0x5D, (byte_t)output[2] );
  EXPECT_EQ( 0x30, (byte_t)output[3] );
}