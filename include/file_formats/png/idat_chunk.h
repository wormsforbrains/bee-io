//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_IDAT_CHUNK_H
#define PNG_IO_IDAT_CHUNK_H

#include "chunk.h"
#include "zlib.h"

#include <iostream>

class IDATChunk : public Chunk {
  private:
    // The original data
    uint8_t *_original_data;
    uint32_t _original_data_length;
    // The compressed data
    Bytef *_compressed_data;
    uLongf _compressed_data_length;

    void _init(ConstructorType type);

  public:
    // Encoding constructor
    IDATChunk(uint32_t length, uint8_t *data);

    // Decoding constructor
    explicit IDATChunk(std::ifstream &stream);

    void encoding_init() override;
    void decoding_init() override;
};

#endif // PNG_IO_IDAT_CHUNK_H
