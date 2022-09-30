#ifndef MYCRAFT_CHUNK_HPP
#define MYCRAFT_CHUNK_HPP

#include "common.hpp"

// NOTE(fonsi): chunks are 32x32x32 blocks in size
#define CHUNK_BLOCK_COUNT (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH)

enum BLOCK_TYPE {
    AIR = 0,
    GRASS,
    STONE,
    SAND,
    SNOW
};

struct Chunk {
    uint8 blocks[CHUNK_BLOCK_COUNT];
    glm::vec2 chunk_position;
    glm::vec2 world_position;

    void init(glm::vec2 chunk_pos);
    void gen();
};

#define WORLD_CHUNK_WIDTH 9
#define WORLD_CHUNK_COUNT (WORLD_CHUNK_WIDTH * WORLD_CHUNK_WIDTH)

struct World {
    Chunk chunks[WORLD_CHUNK_COUNT];
    void gen();
};

extern World world;

#endif
