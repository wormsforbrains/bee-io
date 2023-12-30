//
// Created by Kai on 11/14/2023.
//

#ifndef PNG_IO_CRC_32_H
#define PNG_IO_CRC_32_H

#include <cstdint>

class CRC32 {
    private:
        static uint32_t crc_table[256];
        static bool crc_table_computed;

        static void make_crc_table();

    public:
        static uint32_t update_crc(uint32_t crc, const uint8_t *buf, uint32_t len);
        static uint32_t crc(const uint8_t *buf, uint32_t len);
};

#endif // PNG_IO_CRC_32_H
