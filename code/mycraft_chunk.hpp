#ifndef MYCRAFT_CHUNK_HPP
#define MYCRAFT_CHUNK_HPP

#include "common.hpp"
#include "mycraft_mesh.hpp"

// CHUNK stuff
// NOTE(fonsi): chunks are 32x32x32 blocks in size

struct Chunk {
    uint8 blocks[CHUNK_BLOCK_COUNT];
    Mesh  chunkmesh;
    glm::vec2 world_position;
    glm::vec2 chunk_position;

    void init(glm::vec2 cpos);
    void generate_mesh();
    void render();
};

// WORLD stuff
struct World {
    Chunk chunks[WORLD_CHUNK_COUNT];
    bool has_world_changed;
    glm::vec2 center_chunk;

    void init();
    void render();
};

#endif
