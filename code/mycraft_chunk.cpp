#include "mycraft_chunk.hpp"

// TODO?(fonsi): move global world out of here to another struct or to main file
World world;

#define for_each_chunk(_i, _j) \
    for(uint32 _i = 0; _i < WORLD_CHUNK_WIDTH; _i++) \
        for(uint32 _j = 0; _j < WORLD_CHUNK_WIDTH; _j++) \

#define for_each_block(_i, _j, _k) \
    for(uint32 _i = 0; _i < CHUNK_WIDTH; _i++) \
        for(uint32 _j = 0; _j < CHUNK_WIDTH; _j++) \
            for(uint32 _k = 0; _k < CHUNK_WIDTH; _k++) \

internal inline uint32
block_coords_to_pos(uint32 i, uint32 j, uint32 k)
{
    return(i + j * CHUNK_WIDTH + k * CHUNK_WIDTH * CHUNK_WIDTH);
}
internal inline uint32
chunk_coords_to_pos(uint32 i, uint32 j)
{
    return(i + j * WORLD_CHUNK_WIDTH);
}

void
Chunk::init(glm::vec2 chunk_pos)
{
    chunk_position = chunk_pos;
    world_position[0] = chunk_pos[0] * CHUNK_WIDTH;
    world_position[1] = chunk_pos[1] * CHUNK_WIDTH;
}

void
Chunk::gen()
{
    for_each_block(x, y, z) {
        if(y == 0)
            blocks[block_coords_to_pos(x, y, z)] = BLOCK_TYPE::GRASS;
    }
}

void
World::gen()
{
    for_each_chunk(i, j) {
        chunks[chunk_coords_to_pos(i, j)].init(glm::vec2(i, j));
        chunks[chunk_coords_to_pos(i, j)].gen();
    }
}
