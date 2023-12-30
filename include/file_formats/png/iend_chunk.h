//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_IEND_CHUNK_H
#define PNG_IO_IEND_CHUNK_H

#include "chunk.h"

class IENDChunk : public Chunk {
  private:
    void _init();

  public:
    IENDChunk();

    void encoding_init() override;
};

#endif // PNG_IO_IEND_CHUNK_H
