//
// Created by Kai on 11/14/2023.
//

#include "idat_chunk.h"

#include "crc_32.h"

IDATChunk::IDATChunk(uint32_t length, uint8_t* data) :
    Chunk(0x49444154), _original_data_length(length), _original_data(data),
    _compressed_data(nullptr), _compressed_data_length(0) {
  _init(ENCODING);
}

IDATChunk::IDATChunk(std::ifstream& stream) : Chunk(stream), _original_data(nullptr),
    _original_data_length(0), _compressed_data(nullptr), _compressed_data_length(0) {
  // Get the length
  _compressed_data_length = length();

  // Get the data
  _compressed_data = new Bytef[_compressed_data_length];
  for (uint32_t i = 0; i < _compressed_data_length; i++) {
    _compressed_data[i] = data()[i];
  }

  // Initialize
  _init(DECODING);
}

void IDATChunk::_init(ConstructorType type) {
  if (type == ENCODING) {
    encoding_init();
  } else {
    decoding_init();
  }
}

void IDATChunk::encoding_init() {
  // Create the deflate stream
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  // Initialize the deflate stream
  if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
    throw std::runtime_error("Failed to initialize deflate stream");
  }

  // Compress the data
  _compressed_data_length = _original_data_length + (_original_data_length / 1000) + 12;
  _compressed_data = new uint8_t[_compressed_data_length];
  stream.avail_in = _original_data_length;
  stream.next_in = _original_data;
  stream.avail_out = _compressed_data_length;
  stream.next_out = _compressed_data;
  if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
    throw std::runtime_error("Failed to compress data");
  }
  _compressed_data_length = _compressed_data_length - stream.avail_out;

  // Clean up the deflate stream
  if (deflateEnd(&stream) != Z_OK) {
    throw std::runtime_error("Failed to clean up deflate stream");
  }

  // Set the length
  length(_compressed_data_length);

  // Set the data
  data(_compressed_data);

  // Concatenate the chunk type and data
  auto* chunk_type_and_data = new uint8_t[4 + _compressed_data_length];
  chunk_type_and_data[0] = (_type >> 24) & 0xff;
  chunk_type_and_data[1] = (_type >> 16) & 0xff;
  chunk_type_and_data[2] = (_type >> 8) & 0xff;
  chunk_type_and_data[3] = _type & 0xff;
  for (uint32_t i = 0; i < _compressed_data_length; i++) {
    chunk_type_and_data[i + 4] = _compressed_data[i];
  }

  // Calculate the CRC
  crc(CRC32::crc(chunk_type_and_data, 4 + _compressed_data_length));
}

void IDATChunk::decoding_init() {
  // Create the inflate stream
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  // Initialize the inflate stream
  if (inflateInit(&stream) != Z_OK) {
    throw std::runtime_error("Failed to initialize inflate stream");
  }

  // Decompress the data
  _original_data_length = _compressed_data_length * 1000;
  _original_data = new uint8_t[_original_data_length];
  stream.avail_in = _compressed_data_length;
  stream.next_in = _compressed_data;
  stream.avail_out = _original_data_length;
  stream.next_out = _original_data;
  if (inflate(&stream, Z_FINISH) != Z_STREAM_END) {
    throw std::runtime_error("Failed to decompress data");
  }
  _original_data_length = _original_data_length - stream.avail_out;

  // Clean up the inflate stream
  if (inflateEnd(&stream) != Z_OK) {
    throw std::runtime_error("Failed to clean up inflate stream");
  }
}
