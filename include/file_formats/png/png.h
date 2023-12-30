//
// Created by Kai on 11/15/2023.
//

#ifndef PNG_IO_PNG_H
#define PNG_IO_PNG_H

#include "crc_32.h"
#include "idat_chunk.h"
#include "iend_chunk.h"
#include "ihdr_chunk.h"

#include <cstdint>
#include <fstream>

class PNG {
  private:
    static void _filter_data(uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type, uint8_t filter_method,
        uint8_t* data, uint8_t*& filtered_data, uint32_t* filtered_data_length);

  public:
    static void write_signature(std::ofstream& stream);
    static void write_ihdr(std::ofstream& stream, uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type,
        uint8_t compression_method, uint8_t filter_method, uint8_t interlace_method);
    static void write_idat(std::ofstream& stream, uint32_t width, uint32_t height, uint8_t bit_depth, uint8_t color_type,
        uint8_t filter_method, uint8_t* data);
    static void write_iend(std::ofstream& stream);

    static bool is_png(std::ifstream& stream);
};

#endif // PNG_IO_PNG_H
