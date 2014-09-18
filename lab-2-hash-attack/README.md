CS465 - Lab 2 - Hash Attack
===================

BYU CS 465 Lab 2


Results:


```
[==========] Running 4 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 4 tests from HashAttackTests
[ RUN      ] HashAttackTests.can_hash_any_string_to_tiny
[       OK ] HashAttackTests.can_hash_any_string_to_tiny (0 ms)
[ RUN      ] HashAttackTests.can_generate_random_string
[       OK ] HashAttackTests.can_generate_random_string (0 ms)
[ RUN      ] HashAttackTests.can_generate_tiny_hash_deterministic
[       OK ] HashAttackTests.can_generate_tiny_hash_deterministic (0 ms)
[ RUN      ] HashAttackTests.run_hash_attack
Time: 30 seconds
Attempts: 6584984
Collisions: 6286
[       OK ] HashAttackTests.run_hash_attack (32237 ms)
[----------] 4 tests from HashAttackTests (32237 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test case ran. (32237 ms total)
[  PASSED  ] 4 tests.
```