#include "test_helper.h"
#include "operators.h"

TEST(MathTests, x_time) {
  
  EXPECT_EQ( 0xAE , x_time(0x57) );
  
  EXPECT_EQ( 0x47 , x_time(0xAE) );
  
  EXPECT_EQ( 0x8E , x_time(0x47) );
  
  EXPECT_EQ( 0x07 , x_time(0x8E) );
  
}

TEST(MathTests, ff_multiply) {

  EXPECT_EQ( 0xAE , ff_multiply(0x57, 0x02) );

  EXPECT_EQ( 0x07 , ff_multiply(0x57, 0x10) );

  EXPECT_EQ( 0xA9 , ff_multiply(0x57, 0x12) );
  
  EXPECT_EQ( 0xFE , ff_multiply(0x57, 0x13) );

  EXPECT_EQ( 0xB3 , ff_multiply(0x02, 0xD4) );
  
}

TEST(MathTests, ff_add) {
  
  EXPECT_EQ( 0xD4 , ff_add(0x57, 0x83) );

  EXPECT_EQ( 0xA9 , ff_add(0xAE, 0x07) );
  
  EXPECT_EQ( 0xFE , ff_add(0x57,
                           ff_add(
                                  ff_multiply(0x57, 0x02),
                                  ff_multiply(0x57, 0x10)) ) );
  
}

TEST(MathTests, can_add_and_multiply_together) {

  auto result = ff_add( (ff_multiply(0x02, 0xD4)),
                        (ff_multiply(0x03, 0xBF)),
                        0x5D,
                        0x30 );

  EXPECT_EQ( 0x04, result );
}