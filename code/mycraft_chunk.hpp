#ifndef MYCRAFT_CHUNK_HPP
#define MYCRAFT_CHUNK_HPP

#include "common.hpp"

enum CUBE_TYPE {
    NONE = 0,
    GRASS,
    WATER,
    ROCK,
    SAND,
    SNOW,
};

local const int8 CHUNK_WIDTH = 8;

struct Chunk {
    uint8* blocks;
    uint32 position;

    uint32 vao, vbo, ibo;

    void init();
    void destroy();
};

#endif
