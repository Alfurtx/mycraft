#ifndef MYCRAFT_CHUNK_HPP
#define MYCRAFT_CHUNK_HPP

#include "common.hpp"
#include "mycraft_mesh.hpp"

struct Chunk {
    uint8 blocks[CHUNK_BLOCK_COUNT];
    bool remesh;
    Mesh  chunkmesh;
    glm::vec2 world_position;
    glm::vec2 chunk_position;

    void init(glm::vec2 cpos);
    void generate_mesh();
    void render();
};

struct World {
    Chunk chunks[WORLD_CHUNK_COUNT];
    glm::vec2 center_chunk;

    void init();
    void render();
};

#endif
