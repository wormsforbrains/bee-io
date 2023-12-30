//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_IMAGE_H
#define PNG_IO_IMAGE_H

#include "png.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <stdexcept>

class Image {
  private:
    int _width;
    int _height;
    int _channels;
    uint8_t* _data;

    Image(int width, int height, int channels, uint8_t* data);

  public:
    Image() = default;

    static Image create(int width, int height, int channels);
    static Image create(int width, int height, int channels, uint8_t* data);
    static Image create(int width, int height, int channels, std::initializer_list<uint8_t> fill);

    static Image open(const char* filename);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int channels() const;
    uint8_t* data();
    uint8_t* get_pixel(int x, int y);

    void set_pixel(int x, int y, std::initializer_list<uint8_t> color);
    void set_pixel(int x, int y, uint8_t l);
    void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    void rotate(int angle);

    void save(const char* filename);
};

#endif // PNG_IO_IMAGE_H
