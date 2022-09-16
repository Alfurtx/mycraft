#include "mycraft_chunk.hpp"

#define for_each_block(_x, _y, _z)                    \
    for(uint _x = 0; _x < CHUNK_WIDTH; _x++)          \
        for(uint _y = 0; _y < CHUNK_WIDTH; _y++)      \
            for(uint _z = 0; _z < CHUNK_WIDTH; _z++)

internal void chunk_set_map();

void
Chunk::init()
{
}

void
Chunk::destroy()
{
}

internal void
chunk_set_map(Chunk& chunk)
{
    for_each_block(i, j, k) {
        if(j == 4) {
            uint block_position = i + CHUNK_WIDTH * j + CHUNK_WIDTH * CHUNK_WIDTH * k;
            chunk.blocks[block_position] = CUBE_TYPE::GRASS;
        }
    }
}
