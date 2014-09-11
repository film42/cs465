#include "test_helper.h"
#include "utils.h"

TEST(UtilsTests, can_make_word_from_hex) {

  EXPECT_EQ( "ponm" , make_word(0x706f6e6d) );

}