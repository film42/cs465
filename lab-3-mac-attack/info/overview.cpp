//
// SHA 1 test
//

SHA1 sha1;

sha1.set_iv("f4b645e89faaec2ff8e443c595009c16dbdfba4b");

std::string original = std::string( message_1.begin(), message_1.end() );
std::string extension = "P. S. Give Garrett 100%";
uint64_t message_2_bit_size = extension.length() * 8;
uint64_t message_1_bit_size = original.length() * 8;

EXPECT_EQ( 376, message_1_bit_size );

uint64_t key_length = 128;
uint64_t length_size = 64;

uint64_t message_1_padding = 1024 - (message_1_bit_size + key_length + length_size);
uint64_t message_1_full_length = key_length + message_1_bit_size + message_1_padding + length_size; // 376 + 128 + 456 + 64
uint64_t length = message_1_full_length + message_2_bit_size;

sha1.update( extension );

std::cout << "Padding: " << message_1_padding << std::endl;

EXPECT_EQ( 456 , padding(message_1_padding).length() );
EXPECT_EQ( '1' , padding(message_1_padding)[0] );
EXPECT_EQ( '0' , padding(message_1_padding)[1] );

std::ostringstream sever_message_base;
sever_message_base
<< string_to_binary_string(original)
<< padding(message_1_padding + key_length + message_1_bit_size )
// I think this needs to be the ( original message || key ) in bits for size:
<< gen_length( message_1_bit_size + key_length );

std::ostringstream server_message;
server_message
<< sever_message_base.str()
<< string_to_binary_string(extension);


std::cout << "Using hacked length: " << length << std::endl;
std::string digest = sha1.final( length );

std::cout << "Hex: " << binary_string_to_hex_string( server_message.str() ) << std::endl;

std::cout << digest << std::endl;

EXPECT_EQ( 456, message_1_padding );
EXPECT_EQ( message_1_padding, padding(message_1_padding).length() );


 //
 // SHA 1 .CPP
 //

/*
    sha1.cpp - source code of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Grabsch <vog@notjusthosting.com>
*/

#include "sha1.h"
#include <sstream>
#include <iomanip>
#include <fstream>


SHA1::SHA1()
{
    reset();
}


void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


void SHA1::update(std::istream &is) {
  std::string rest_of_buffer;
  read(is, rest_of_buffer, BLOCK_BYTES - buffer.size());
  buffer += rest_of_buffer;

  while (is) {
    uint32 block[BLOCK_INTS];
    buffer_to_block(buffer, block);
    transform(block);
    read(is, buffer, BLOCK_BYTES);
  }
}


std::string SHA1::final(uint64_t length) {

  /* Padding */
  buffer += 0x80;
  unsigned int orig_size = buffer.size();
  while (buffer.size() < BLOCK_BYTES)
  {
    buffer += (char)0x00;
  }

  uint32 block[BLOCK_INTS];
  buffer_to_block(buffer, block);

  if (orig_size > BLOCK_BYTES - 8)
  {
    transform(block);
    for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
    {
      block[i] = 0;
    }
  }

  /* Append total_bits, split this uint64 into two uint32 */
  block[BLOCK_INTS - 1] = length;
  block[BLOCK_INTS - 2] = (length >> 32);

  transform(block);

  /* Hex std::string */
  std::ostringstream result;
  for (unsigned int i = 0; i < DIGEST_INTS; i++)
  {
    result << std::hex << std::setfill('0') << std::setw(8);
    result << (digest[i] & 0xffffffff);
  }

  /* Reset for next run */
  reset();

  return result.str();
}


/*
 * Add padding and return the message digest.
 */

std::string SHA1::final() {

  /* Total number of hashed bits */
  uint64_t total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

  return final( total_bits );
}


std::string SHA1::from_file(const std::string &filename)
{
    std::ifstream stream(filename.c_str(), std::ios::binary);
    SHA1 checksum;
    checksum.update(stream);
    return checksum.final();
}


void SHA1::set_iv(std::string iv) {

  /* SHA1 initialization constants */
  digest[0] = hex_string_to_int( iv.substr(0, 8) );
  digest[1] = hex_string_to_int( iv.substr(8, 8) );
  digest[2] = hex_string_to_int( iv.substr(16, 8) );
  digest[3] = hex_string_to_int( iv.substr(24, 8) );
  digest[4] = hex_string_to_int( iv.substr(32) );

}


void SHA1::reset()
{
    /* SHA1 initialization constants */
    digest[0] = 0x67452301;
    digest[1] = 0xefcdab89;
    digest[2] = 0x98badcfe;
    digest[3] = 0x10325476;
    digest[4] = 0xc3d2e1f0;

    /* Reset counters */
    transforms = 0;
    buffer = "";
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

void SHA1::transform(uint32 block[BLOCK_BYTES])
{
    /* Copy digest[] to working vars */
    uint32 a = digest[0];
    uint32 b = digest[1];
    uint32 c = digest[2];
    uint32 d = digest[3];
    uint32 e = digest[4];

    /* Help macros */
#define rol(value, bits) (((value) << (bits)) | (((value) & 0xffffffff) >> (32 - (bits))))
#define blk(i) (block[i&15] = rol(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i&15],1))

    /* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i] + 0x5a827999 + rol(v,5); w=rol(w,30);
#define R1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + blk(i)   + 0x5a827999 + rol(v,5); w=rol(w,30);
#define R2(v,w,x,y,z,i) z += (w^x^y)           + blk(i)   + 0x6ed9eba1 + rol(v,5); w=rol(w,30);
#define R3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + blk(i)   + 0x8f1bbcdc + rol(v,5); w=rol(w,30);
#define R4(v,w,x,y,z,i) z += (w^x^y)           + blk(i)   + 0xca62c1d6 + rol(v,5); w=rol(w,30);

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(a,b,c,d,e, 0);
    R0(e,a,b,c,d, 1);
    R0(d,e,a,b,c, 2);
    R0(c,d,e,a,b, 3);
    R0(b,c,d,e,a, 4);
    R0(a,b,c,d,e, 5);
    R0(e,a,b,c,d, 6);
    R0(d,e,a,b,c, 7);
    R0(c,d,e,a,b, 8);
    R0(b,c,d,e,a, 9);
    R0(a,b,c,d,e,10);
    R0(e,a,b,c,d,11);
    R0(d,e,a,b,c,12);
    R0(c,d,e,a,b,13);
    R0(b,c,d,e,a,14);
    R0(a,b,c,d,e,15);
    R1(e,a,b,c,d,16);
    R1(d,e,a,b,c,17);
    R1(c,d,e,a,b,18);
    R1(b,c,d,e,a,19);
    R2(a,b,c,d,e,20);
    R2(e,a,b,c,d,21);
    R2(d,e,a,b,c,22);
    R2(c,d,e,a,b,23);
    R2(b,c,d,e,a,24);
    R2(a,b,c,d,e,25);
    R2(e,a,b,c,d,26);
    R2(d,e,a,b,c,27);
    R2(c,d,e,a,b,28);
    R2(b,c,d,e,a,29);
    R2(a,b,c,d,e,30);
    R2(e,a,b,c,d,31);
    R2(d,e,a,b,c,32);
    R2(c,d,e,a,b,33);
    R2(b,c,d,e,a,34);
    R2(a,b,c,d,e,35);
    R2(e,a,b,c,d,36);
    R2(d,e,a,b,c,37);
    R2(c,d,e,a,b,38);
    R2(b,c,d,e,a,39);
    R3(a,b,c,d,e,40);
    R3(e,a,b,c,d,41);
    R3(d,e,a,b,c,42);
    R3(c,d,e,a,b,43);
    R3(b,c,d,e,a,44);
    R3(a,b,c,d,e,45);
    R3(e,a,b,c,d,46);
    R3(d,e,a,b,c,47);
    R3(c,d,e,a,b,48);
    R3(b,c,d,e,a,49);
    R3(a,b,c,d,e,50);
    R3(e,a,b,c,d,51);
    R3(d,e,a,b,c,52);
    R3(c,d,e,a,b,53);
    R3(b,c,d,e,a,54);
    R3(a,b,c,d,e,55);
    R3(e,a,b,c,d,56);
    R3(d,e,a,b,c,57);
    R3(c,d,e,a,b,58);
    R3(b,c,d,e,a,59);
    R4(a,b,c,d,e,60);
    R4(e,a,b,c,d,61);
    R4(d,e,a,b,c,62);
    R4(c,d,e,a,b,63);
    R4(b,c,d,e,a,64);
    R4(a,b,c,d,e,65);
    R4(e,a,b,c,d,66);
    R4(d,e,a,b,c,67);
    R4(c,d,e,a,b,68);
    R4(b,c,d,e,a,69);
    R4(a,b,c,d,e,70);
    R4(e,a,b,c,d,71);
    R4(d,e,a,b,c,72);
    R4(c,d,e,a,b,73);
    R4(b,c,d,e,a,74);
    R4(a,b,c,d,e,75);
    R4(e,a,b,c,d,76);
    R4(d,e,a,b,c,77);
    R4(c,d,e,a,b,78);
    R4(b,c,d,e,a,79);

    /* Add the working vars back into digest[] */
    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;

    /* Count the number of transformations */
    transforms++;
}


void SHA1::buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES])
{
    /* Convert the std::string (byte buffer) to a uint32 array (MSB) */
    for (unsigned int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (buffer[4*i+3] & 0xff)
                   | (buffer[4*i+2] & 0xff)<<8
                   | (buffer[4*i+1] & 0xff)<<16
                   | (buffer[4*i+0] & 0xff)<<24;
    }
}


void SHA1::read(std::istream &is, std::string &s, int max)
{
    char sbuf[max];
    is.read(sbuf, max);
    s.assign(sbuf, is.gcount());
}





//
// Utils.cpp
//


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
