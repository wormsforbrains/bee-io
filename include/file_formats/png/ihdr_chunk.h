//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_IHDR_CHUNK_H
#define PNG_IO_IHDR_CHUNK_H

#include "chunk.h"

#include <stdexcept>

class IHDRChunk : public Chunk {
  private:
    uint32_t _width;
    uint32_t _height;
    uint8_t _bit_depth;
    uint8_t _color_type;
    uint8_t _compression_method;
    uint8_t _filter_method;
    uint8_t _interlace_method;

    void _validate();

  public:
    // Encoding constructor
    IHDRChunk(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t compression_method,
        uint8_t filter_method, uint8_t interlace_method);

    // Getters
    [[nodiscard]] uint32_t width() const;
    [[nodiscard]] uint32_t height() const;
    [[nodiscard]] uint8_t bit_depth() const;
    [[nodiscard]] uint8_t color_type() const;
    [[nodiscard]] uint8_t compression_method() const;
    [[nodiscard]] uint8_t filter_method() const;
    [[nodiscard]] uint8_t interlace_method() const;

    void encoding_init() override;
};

#endif // PNG_IO_IHDR_CHUNK_H
