//
// Created by Kai on 11/15/2023.
//

#include "png.h"

void PNG::write_signature(std::ofstream& stream) {
  // Write the signature to the stream
  stream << "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a";
}

void PNG::write_ihdr(std::ofstream& stream, uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type,
    uint8_t compression_method, uint8_t filter_method, uint8_t interlace_method) {
  // Create the IHDR chunk
  IHDRChunk ihdr_chunk(width, height, bit_depth, color_type, compression_method, filter_method, interlace_method);

  // Write the IHDR chunk to the stream
  ihdr_chunk.write(stream);
}

void PNG::write_idat(std::ofstream& stream, uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type,
    uint8_t filter_method, uint8_t* data) {
  // Filter the data
  uint8_t* filtered_data;
  uint32_t filtered_data_length;
  _filter_data(width, height, bit_depth, color_type, filter_method, data, filtered_data, &filtered_data_length);

  // Create the IDAT chunk
  IDATChunk idat_chunk(filtered_data_length, filtered_data);

  // Write the IDAT chunk to the stream
  idat_chunk.write(stream);
}

void PNG::write_iend(std::ofstream& stream) {
  // Create the IEND chunk
  IENDChunk iend_chunk;

  // Write the IEND chunk to the stream
  iend_chunk.write(stream);
}

bool PNG::is_png(std::ifstream& stream) {
  // Read the signature from the stream
  char signature[8];
  stream.read(signature, 8);

  // Check if the signature is valid
  return signature[0] == '\x89' && signature[1] == 'P' && signature[2] == 'N' && signature[3] == 'G' && signature[4] == '\r' && signature[5] == '\n' && signature[6] == '\x1a' && signature[7] == '\n';
}

void PNG::_filter_data(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t filter_method,
    uint8_t* data, uint8_t*& filtered_data, uint32_t* filtered_data_length) {
  // Preliminary implementation with only prepending a 0 byte to each scanline
  // Assume rgb color type
  uint32_t scanline_length = width * 3;
  uint32_t filtered_scanline_length = scanline_length + 1;
  uint32_t filtered_data_length_ = (width * height * 3) + height;
  auto* filtered_data_ = new uint8_t[filtered_data_length_];

  // Filter each scanline
  for (uint32_t y = 0; y < height; y++) {
    // Prepend a 0 byte to the scanline
    filtered_data_[y * filtered_scanline_length + 0] = 0;
    for (uint32_t x = 0; x < scanline_length; x++) {
      filtered_data_[y * filtered_scanline_length + x + 1] = data[y * scanline_length + x];
    }
  }

  // Set the filtered data length
  *filtered_data_length = filtered_data_length_;

  // Set the filtered data
  filtered_data = filtered_data_;
}
