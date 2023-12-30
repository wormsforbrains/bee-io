//
// Created by Kai on 11/14/2023.
//

#include "chunk.h"

#include <winsock.h>

// Constructors
Chunk::Chunk(uint32_t type) :
    _length(0), _type(type), _data(nullptr), _crc(0xffffffffL) {}

Chunk::Chunk(std::ifstream& stream) :
    _length(0), _type(0), _data(nullptr), _crc(0xffffffffL) {}

// Destructor
Chunk::~Chunk() {
  delete[] _data;
}

// Accessors
uint32_t Chunk::length() const { return _length; }
uint32_t Chunk::type() const { return _type; }
uint8_t* Chunk::data() { return _data; }
uint32_t Chunk::crc() const { return _crc; }

// Mutators
void Chunk::length(uint32_t length) { _length = length; }
void Chunk::type(uint32_t type) { _type = type; }
void Chunk::data(uint8_t* data) { _data = data; }
void Chunk::crc(uint32_t crc) { _crc = crc; }

// Methods
void Chunk::write(std::ofstream& stream) {
  // most systems are little-endian, so we need to convert to big-endian
  uint32_t length = htonl(_length);
  uint32_t type = htonl(_type);
  uint32_t crc = htonl(_crc);

  // write the length, type, and data to the stream
  stream.write((char*)&length, sizeof(uint32_t));
  stream.write((char*)&type, sizeof(uint32_t));
  if (_length > 0) {
    stream.write((char*)_data, _length);
  }
  stream.write((char*)&crc, sizeof(uint32_t));
}
