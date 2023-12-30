//
// Created by Kai on 11/14/2023.
//

#include "iend_chunk.h"

#include "crc_32.h"

IENDChunk::IENDChunk() :
    Chunk(0x49454E44) { _init(); }

void IENDChunk::_init() { encoding_init(); }

void IENDChunk::encoding_init() {
  _length = 0;
  _data = nullptr;

  uint8_t type[4];
  type[0] = (_type >> 24) & 0xff;
  type[1] = (_type >> 16) & 0xff;
  type[2] = (_type >> 8) & 0xff;
  type[3] = _type & 0xff;
  _crc = CRC32::crc(type, 4);
}

void IENDChunk::decoding_init() {
  // Nothing to do here
}
