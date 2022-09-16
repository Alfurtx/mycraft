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

struct Chunk {
    uint8* blocks;
    uint32 position;

    void init();
    void destroy();
};

#endif
