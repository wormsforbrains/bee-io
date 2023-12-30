//
// Created by Kai on 11/14/2023.
//

#include "ihdr_chunk.h"

#include "crc_32.h"

IHDRChunk::IHDRChunk(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t compression_method,
    uint8_t filter_method, uint8_t interlace_method) :
    Chunk(0x49484452),
    _width(width), _height(height), _bit_depth(bit_depth), _color_type(color_type),
    _compression_method(compression_method), _filter_method(filter_method), _interlace_method(interlace_method) {
  _length = 13;
  _validate(ENCODING);
}

IHDRChunk::IHDRChunk(std::ifstream& stream) :
    Chunk(0x49484452) {
  // Read the width
  _width = (stream.get() << 24) | (stream.get() << 16) | (stream.get() << 8) | stream.get();

  // Read the height
  _height = (stream.get() << 24) | (stream.get() << 16) | (stream.get() << 8) | stream.get();

  // Read the bit depth
  _bit_depth = stream.get();

  // Read the color type
  _color_type = stream.get();

  // Read the compression method
  _compression_method = stream.get();

  // Read the filter method
  _filter_method = stream.get();

  // Read the interlace method
  _interlace_method = stream.get();

  // Read the CRC
  _crc = (stream.get() << 24) | (stream.get() << 16) | (stream.get() << 8) | stream.get();

  // Validate the chunk
  _validate(DECODING);
}

uint32_t IHDRChunk::width() const { return _width; }
uint32_t IHDRChunk::height() const { return _height; }
uint8_t IHDRChunk::bit_depth() const { return _bit_depth; }
uint8_t IHDRChunk::color_type() const { return _color_type; }
uint8_t IHDRChunk::compression_method() const { return _compression_method; }
uint8_t IHDRChunk::filter_method() const { return _filter_method; }
uint8_t IHDRChunk::interlace_method() const { return _interlace_method; }

void IHDRChunk::_validate(ConstructorType type) {
  std::string errors;

  // Check the length
  if (_length != 13) {
    errors += "length is not 13; ";
  }

  // Check the width
  if (_width == 0) {
    errors += "width is 0; ";
  }

  // Check the height
  if (_height == 0) {
    errors += "height is 0; ";
  }

  // Check the color type / bit depth combination
  if (_color_type == 0) {
    if (_bit_depth != 1 && _bit_depth != 2 && _bit_depth != 4 && _bit_depth != 8 && _bit_depth != 16) {
      errors += "color type is 0 but bit depth is not 1, 2, 4, 8, or 16; ";
    }
  } else if (_color_type == 2) {
    if (_bit_depth != 8 && _bit_depth != 16) {
      errors += "color type is 2 but bit depth is not 8 or 16; ";
    }
  } else if (_color_type == 3) {
    if (_bit_depth != 1 && _bit_depth != 2 && _bit_depth != 4 && _bit_depth != 8) {
      errors += "color type is 3 but bit depth is not 1, 2, 4, or 8; ";
    }
  } else if (_color_type == 4) {
    if (_bit_depth != 8 && _bit_depth != 16) {
      errors += "color type is 4 but bit depth is not 8 or 16; ";
    }
  } else if (_color_type == 6) {
    if (_bit_depth != 8 && _bit_depth != 16) {
      errors += "bit depth is not 8 or 16; ";
    }
  } else {
    errors += "color type is not 0, 2, 3, 4, or 6; ";
  }

  // Check the compression method
  if (_compression_method != 0) {
    errors += "compression method is not 0; ";
  }

  // Check the filter method
  if (_filter_method != 0) {
    errors += "filter method is not 0; ";
  }

  // Check the interlace method
  if (_interlace_method != 0 && _interlace_method != 1) {
    errors += "interlace method is not 0 or 1; ";
  }

  // If there are any errors, throw an exception
  if (!errors.empty()) {
    throw std::runtime_error("IHDR chunk is invalid: " + errors);
  }

  // Initialize the chunk
  if (type == ENCODING) {
    encoding_init();
  } else {
    decoding_init();
  }
}

void IHDRChunk::encoding_init() {
  // Allocate memory for the data
  _data = new uint8_t[_length];

  // Write the image width,
  _data[0] = (_width >> 24) & 0xFF;
  _data[1] = (_width >> 16) & 0xFF;
  _data[2] = (_width >> 8) & 0xFF;
  _data[3] = _width & 0xFF;

  // image height,
  _data[4] = (_height >> 24) & 0xFF;
  _data[5] = (_height >> 16) & 0xFF;
  _data[6] = (_height >> 8) & 0xFF;
  _data[7] = _height & 0xFF;

  // bit depth,
  _data[8] = _bit_depth;

  // color type,
  _data[9] = _color_type;

  // compression method,
  _data[10] = _compression_method;

  // filter method,
  _data[11] = _filter_method;

  // and interlace method
  _data[12] = _interlace_method;

  // Concatenate the type and data fields
  uint8_t type_and_data[17];
  type_and_data[0] = (_type >> 24) & 0xFF;
  type_and_data[1] = (_type >> 16) & 0xFF;
  type_and_data[2] = (_type >> 8) & 0xFF;
  type_and_data[3] = _type & 0xFF;
  for (int i = 0; i < 13; i++) {
    type_and_data[i + 4] = _data[i];
  }

  // Calculate the CRC
  _crc = CRC32::crc(type_and_data, 17);
}

void IHDRChunk::decoding_init() {
  // Nothing to do here
}
