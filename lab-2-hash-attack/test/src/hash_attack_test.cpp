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

  EXPECT_EQ( random_string_1 , random_string_2 );
}

TEST(HashAttackTests, run_hash_attack) {

  auto database = Database::get_instance();
  auto bit_count = 8;
  int collision_count = 0;
  int test_size = 10;
  uint64_t attempts = 0;
  uint64_t prediction = 0;

  while( collision_count < test_size ) {

    auto a_random_string = random_string();

    if( database->original_exists( a_random_string ) ) {
      continue;
    }

    ++attempts;

    auto hash = tiny_sha1( bit_count, a_random_string );
    database->insert( a_random_string , hash );

    if( database->collision_for_hash( hash ) ) {
      ++collision_count;

      // DEBUG
      if( collision_count == 1 ) {
        prediction = attempts;
        std::cout << "Predicted: " << attempts << std::endl;
      }
    }

  }

  std::cout << "Average: " << (attempts / prediction) << std::endl;
  std::cout << "Attempts: " << attempts << std::endl;
  std::cout << "Collisions: " << database->collision_count() << std::endl;
}