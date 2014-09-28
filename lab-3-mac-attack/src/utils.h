#ifndef __utils__
#define __utils__

#include <cstdint>
#include <bitset>
#include <sstream>


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

  static std::string binary_string_to_hex_string(std::string binary_string) {

    if( binary_string.length() % 8 != 0) {
      return "";
    }

    std::ostringstream ostream;

    for(int i = 0; i < binary_string.length(); i += 8) {

      ostream << std::hex << std::bitset<8>( binary_string.substr(i, 8) ).to_ulong();

    }

    return ostream.str();
  }

  static std::string string_to_binary_string(std::string input) {
    std::ostringstream ostream;

    for (size_t i = 0; i < input.size(); ++i) {
      ostream << std::bitset<8>(input.c_str()[i]).to_string();
    }

    return ostream.str();
  }

  static std::string padding(uint64_t size) {

    std::string result = "1";

    // Append until the end
    for(; 0 < size - 1; --size) {
      result += "0";
    }

    return result;
  }

  static std::string gen_length(uint64_t size) {
    std::cout << "Generating size: " << size << std::endl;
    return std::bitset<64>( size ).to_string();
  }

}


#endif