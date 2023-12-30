//
// Created by Kai on 11/14/2023.
//

#include "crc_32.h"
#include "utils.h"

#include <functional>
#include <iostream>

enum Endianness {
  BIG_ENDIAN,
  LITTLE_ENDIAN
};

// Print a 32-bit integer in hexadecimal
void print_hex(uint32_t n, Endianness endianness) {
  // Example: 0x87654321
  uint8_t bytes[4];
  bytes[0] = (n >> 24) & 0xff; // 0x87
  bytes[1] = (n >> 16) & 0xff; // 0x65
  bytes[2] = (n >> 8) & 0xff;  // 0x43
  bytes[3] = n & 0xff;         // 0x21

  if (endianness == BIG_ENDIAN) {
    // Example number would be printed as 87654321
    std::cout << std::hex << (int)bytes[0] << (int)bytes[1] << (int)bytes[2] << (int)bytes[3] << std::endl;
  } else {
    // Example number would be printed as 21436587
    std::cout << std::hex << (int)bytes[3] << (int)bytes[2] << (int)bytes[1] << (int)bytes[0] << std::endl;
  }
}

int main() {
  uint8_t data[] = { 0x49, 0x45, 0x4E, 0x44 };

  uint32_t crc = CRC32::crc(data, 4);
  print_hex(crc, BIG_ENDIAN);

  return 0;
}
