#include "test_helper.h"
#include "box.h"

TEST(BoxTests, can_get_sbox) {
  
  EXPECT_EQ( sbox_get(0x46) , 0x5a );
  
}

TEST(BoxTests, can_get_inverse_sbox) {
  
  EXPECT_EQ( inverse_sbox_get(0x5a) , 0x46 );
  
}