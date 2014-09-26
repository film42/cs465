#ifndef __utils__
#define __utils__

#include <cstdint>
#include <bitset>


typedef uint8_t byte_t;

namespace {

  static std::string string_to_hex(std::string input) {
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
      const byte_t c = input[i];
      output.push_back(lut[c >> 4]);
      output.push_back(lut[c & 15]);
    }
    return output;
  }

  static uint32_t hex_string_to_int(std::string hex) {
    return (uint32_t)strtol( hex.c_str(), NULL, 16 );
  }

  static std::string padding(size_t size) {

    std::string result = "1";
    --size;

    // Append until the end
    while(size > 0) {
      result += "0";
      --size;
    }

    return result;
  }

  static std::string gen_length(size_t size) {
    return std::bitset< 64 >( size ).to_string();
  }

}


#endif