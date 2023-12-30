//
// Created by Kai on 11/14/2023.
//

#include "image.h"

#include <random>

// Constructors
Image::Image(int width, int height, int channels, uint8_t* data) :
    _width(width),
    _height(height),
    _channels(channels),
    _data(data) {}

// Static Methods
Image Image::create(int width, int height, int channels) {
  if (width < 1) {
    throw std::invalid_argument("Width must be greater than 0");
  }
  if (height < 1) {
    throw std::invalid_argument("Height must be greater than 0");
  }
  if (channels < 1 || channels > 4) {
    throw std::invalid_argument("Channels must be between 1, 3, or 4");
  }

  auto* data = new uint8_t[width * height * channels];
  for (int i = 0; i < width * height * channels; i++) {
    data[i] = 0;
  }
  return { width, height, channels, data };
}

Image Image::create(int width, int height, int channels, uint8_t* data) {
  if (width < 1) {
    throw std::invalid_argument("Width must be greater than 0");
  }
  if (height < 1) {
    throw std::invalid_argument("Height must be greater than 0");
  }
  if (channels < 1 || channels > 4) {
    throw std::invalid_argument("Channels must be between 1, 3, or 4");
  }
  if (data == nullptr) {
    throw std::invalid_argument("Data must not be null");
  }

  return { width, height, channels, data };
}

Image Image::create(int width, int height, int channels, std::initializer_list<uint8_t> fill) {
  if (width < 1) {
    throw std::invalid_argument("Width must be greater than 0");
  }
  if (height < 1) {
    throw std::invalid_argument("Height must be greater than 0");
  }
  if (channels < 1 || channels > 4) {
    throw std::invalid_argument("Channels must be between 1, 3, or 4");
  }
  if (fill.size() != channels) {
    throw std::invalid_argument("Fill must have the same number of elements as channels");
  }

  auto* data = new uint8_t[width * height * channels];
  for (int i = 0; i < width * height * channels; i++) {
    data[i] = *(fill.begin() + (i % channels));
  }
  return { width, height, channels, data };
}

Image Image::open(const char *filename) {
  throw std::uni

  // Open the file
  std::ifstream file(filename, std::ios::binary);

  // Check if the file is open
  if (!file.is_open()) {
    throw std::invalid_argument("File could not be opened");
  }

  // Check if the file is a PNG
  if (!PNG::is_png(file)) {
    throw std::invalid_argument("File is not a PNG");
  }

  // Read the IHDR chunk
  IHDRChunk ihdr_chunk(file);

  // Read IDAT chunks until the IEND chunk is reached
  std::vector<IDATChunk> idat_chunks;
  while (true) {
    try {
      idat_chunks.emplace_back(file);
    } catch (std::runtime_error& e) {
      break;
    }
  }

  // The only chunk left should be the IEND chunk, which we don't care about
  file.close();

  // Unfilter the data

}

// Accessors
int Image::width() const { return _width; }
int Image::height() const { return _height; }
int Image::channels() const { return _channels; }
uint8_t* Image::data() { return _data; }

uint8_t* Image::get_pixel(int x, int y) {
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    return &_data[(y * _width + x) * _channels];
  }
  return nullptr;
}

// Mutators
void Image::set_pixel(int x, int y, std::initializer_list<uint8_t> color) {
  if (color.size() != _channels) {
    throw std::invalid_argument("Color must have the same number of elements as the image has channels");
  }
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    for (int i = 0; i < _channels; i++) {
      _data[(y * _width + x) * _channels + i] = *(color.begin() + i);
    }
  }
}

void Image::set_pixel(int x, int y, uint8_t l) {
  if (_channels != 1) {
    throw std::invalid_argument("Image must have 1 channel");
  }
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _data[y * _width + x] = l;
  }
}

void Image::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  if (_channels != 3) {
    throw std::invalid_argument("Image must have 3 channels");
  }
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _data[(y * _width + x) * _channels + 0] = r;
    _data[(y * _width + x) * _channels + 1] = g;
    _data[(y * _width + x) * _channels + 2] = b;
  }
}

void Image::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  if (_channels != 4) {
    throw std::invalid_argument("Image must have 4 channels");
  }
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _data[(y * _width + x) * _channels + 0] = r;
    _data[(y * _width + x) * _channels + 1] = g;
    _data[(y * _width + x) * _channels + 2] = b;
    _data[(y * _width + x) * _channels + 3] = a;
  }
}

void Image::rotate(int angle) {
  switch (angle) {
    case 90: {
      auto* data = new uint8_t[_width * _height * _channels];
      for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
          for (int c = 0; c < _channels; c++) {
            data[(x * _height + (_height - y - 1)) * _channels + c] = _data[(y * _width + x) * _channels + c];
          }
        }
      }
      delete[] _data;
      _data = data;
      int temp = _width;
      _width = _height;
      _height = temp;
      break;
    }
    case 180: {
      auto* data = new uint8_t[_width * _height * _channels];
      for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
          for (int c = 0; c < _channels; c++) {
            data[((_height - y - 1) * _width + (_width - x - 1)) * _channels + c] = _data[(y * _width + x) * _channels + c];
          }
        }
      }
      delete[] _data;
      _data = data;
      break;
    }
    case 270: {
      auto* data = new uint8_t[_width * _height * _channels];
      for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
          for (int c = 0; c < _channels; c++) {
            data[((_width - x - 1) * _height + y) * _channels + c] = _data[(y * _width + x) * _channels + c];
          }
        }
      }
      delete[] _data;
      _data = data;
      int temp = _width;
      _width = _height;
      _height = temp;
      break;
    }
    default:
      throw std::invalid_argument("Invalid angle");
  }
}

void Image::save(const char* filename) {
  uint8_t color_type;
  switch (_channels) {
    case 1:
      color_type = 0;
      break;
    case 3:
      color_type = 2;
      break;
    case 4:
      color_type = 6;
      break;
    default:
      throw std::invalid_argument("Invalid number of channels");
  }

  uint8_t bit_depth = 8;
  uint8_t compression_method = 0;
  uint8_t filter_method = 0;
  uint8_t interlace_method = 0;

  // Open the file
  std::ofstream file(filename, std::ios::binary);

  // Write the PNG file
  PNG::write_signature(file);
  PNG::write_ihdr(file, _width, _height, bit_depth, color_type, compression_method, filter_method, interlace_method);
  PNG::write_idat(file, _width, _height, bit_depth, color_type, filter_method, _data);
  PNG::write_iend(file);

  // Close the file
  file.close();
}
