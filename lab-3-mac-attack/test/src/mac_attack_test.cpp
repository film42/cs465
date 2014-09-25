#include <iostream>
#include <cmath>

#include "hash_attack.h"
#include "db.h"
#include "test_helper.h"

TEST(HashAttackTests, can_hash_any_string_to_tiny) {

  auto result1 = tiny_sha1(12, "something longer");

  EXPECT_EQ( 12 , result1.length() );

  auto result2 = tiny_sha1(12, "");

  EXPECT_EQ( 12 , result2.length() );
}

TEST(HashAttackTests, can_generate_random_string) {

  srand( 100 );

  std::string random_string_1 = random_string();

  srand( 100 );

  std::string random_string_2 = random_string();

  std::string different = random_string();

  EXPECT_EQ( random_string_1 , random_string_2 );
  EXPECT_TRUE( different != random_string_2 );
}

TEST(HashAttackTests, tiny_sha1_tests) {

  srand( 100 );

  std::string random_string_1 = random_string();
  auto hash_1 = tiny_sha1(8, random_string_1 );

  srand( 100 );

  std::string random_string_2 = random_string();
  auto hash_2 = tiny_sha1(8, random_string_2 );

  std::string different = random_string();
  auto hash_different = tiny_sha1(8, different);

  EXPECT_EQ( random_string_1 , random_string_2 );
  EXPECT_EQ( hash_1 , hash_2 );
  EXPECT_TRUE( hash_different != hash_1 );
}

TEST(HashAttackTests, run_collision_attack) {

  srand(std::time(0));

  time_t start = std::time(0);

  for(int i = 8; i <= 32; ++i) {

    collision_test( i , 100 );

    std::cout << std::endl;

  }

  std::cout << "Elapsed Time: " << ( std::time(0) - start ) << " Seconds" << std::endl;

}

TEST(HashAttackTests, can_do_pre_image_attack) {
  return;

  srand(std::time(0));

  std::string pre_image = "hello, world! My name is...";

  time_t start = std::time(0);

  for(int i = 8; i <= 16; ++i) {

    pre_image_test( i , 100, pre_image );

    std::cout << std::endl;

  }

  std::cout << "Elapsed Time: " << ( std::time(0) - start ) << " Seconds" << std::endl;


}