//
// Created by Kai on 11/15/2023.
//

#ifndef PNG_IO_DEFLATE_H
#define PNG_IO_DEFLATE_H

#include <cstdint>

/*
 * Important features of the deflate algorithm:
 *
 * 1. Compressed data is a sequence of blocks:
 *   a. [block 1][block 2]...[block n]
 *   b. Blocks sizes are arbitrary, but no block can exceed 65,535 bytes.
 *   c. Each block is encoded independently of the other blocks.
 *   d. Each block is encoded using a combination of Huffman coding and LZ77
 *   compression.
 *   
 */

#endif // PNG_IO_DEFLATE_H
