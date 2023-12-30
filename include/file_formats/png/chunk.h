//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_CHUNK_H
#define PNG_IO_CHUNK_H

#include <cstdint>
#include <fstream>

class Chunk {
  protected:
    uint32_t _length;
    uint32_t _type;
    uint8_t* _data;
    uint32_t _crc;

  public:
    // Encoding constructor
    explicit Chunk(uint32_t type);

    ~Chunk();

    [[nodiscard]] uint32_t length() const;
    [[nodiscard]] uint32_t type() const;
    uint8_t* data();
    [[nodiscard]] uint32_t crc() const;

    void length(uint32_t length);
    void type(uint32_t type);
    void data(uint8_t* data);
    void crc(uint32_t crc);

    virtual void encoding_init() = 0;

    void write(std::ofstream& stream);
};

#endif // PNG_IO_CHUNK_H
