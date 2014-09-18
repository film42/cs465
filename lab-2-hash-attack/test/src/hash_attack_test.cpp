#include <iostream>
#include "hash_attack.h"
#include "db.h"
#include "test_helper.h"

TEST(HashAttackTests, can_hash_any_string_to_tiny) {

  auto result1 = tiny_sha1("something longer");

  EXPECT_EQ( 5 , result1.length() );

  auto result2 = tiny_sha1("");

  EXPECT_EQ( 5 , result2.length() );
}

TEST(HashAttackTests, can_generate_random_string) {

  srand( 100 );

  std::string random_string_1 = random_string();

  srand( 100 );

  std::string random_string_2 = random_string();

  EXPECT_EQ( random_string_1 , random_string_2 );
}

TEST(HashAttackTests, can_generate_tiny_hash_deterministic) {

  srand( 100 );

  std::string random_string_1 = random_tiny_sha1();

  srand( 100 );

  std::string random_string_2 = random_tiny_sha1();

  std::string different = random_tiny_sha1();

  EXPECT_EQ( 5 , random_string_2.length() );
  EXPECT_EQ( random_string_1 , random_string_2 );
  EXPECT_TRUE( different != random_string_2 );
}

TEST(HashAttackTests, run_hash_attack) {

  auto database = Database::get_instance();
  auto future_time = std::time(0) + 30;
  uint64_t attempts = 0;

  while( std::time(0) < future_time ) {

    auto a_random_string = random_string();

    if( database->original_exists( a_random_string ) ) {
      continue;
    }

    ++attempts;

    database->insert( a_random_string , tiny_sha1( a_random_string ) );
  }

  std::cout << "Time: 30 seconds " << std::endl;
  std::cout << "Attempts: " << attempts << std::endl;
  std::cout << "Collisions: " << database->collision_count() << std::endl;
}