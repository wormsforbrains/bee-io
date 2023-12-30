//
// Created by Kai on 11/14/2023.
//

#include "crc_32.h"

uint32_t CRC32::crc_table[256];
bool CRC32::crc_table_computed = false;

void CRC32::make_crc_table() {
    uint32_t c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (uint32_t) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

uint32_t CRC32::update_crc(uint32_t crc, const uint8_t *buf, uint32_t len) {
    uint32_t c = crc;
    uint32_t n;

    if (!crc_table_computed) {
        make_crc_table();
    }
    for (n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

uint32_t CRC32::crc(const uint8_t *buf, uint32_t len) {
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}
