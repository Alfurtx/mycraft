#ifndef MYCRAFT_CHUNK_HPP
#define MYCRAFT_CHUNK_HPP

#include "common.hpp"
#include "mc_blocks.hpp"

/*  3D CUBE
 *  1-------2
 *  |5------+6
 *  ||      ||
 *  ||      ||
 *  0+------3|
 *   4-------7
 *
 *  bottom:
 *  4-------7
 *  |       |
 *  |       |
 *  |       |
 *  0-------3
 *
 * top:
 *  5-------6
 *  |       |
 *  |       |
 *  |       |
 *  1-------2
 *
 * vertices:
 *  (0, 0, 0)
 *  (0, 1, 0)
 *  (1, 1, 0)
 *  (1, 0, 0)
 *  (0, 0, 1)
 *  (0, 1, 1)
 *  (1, 1, 1)
 *  (1, 0, 1)
 *
 * indices:
 * 4, 7, 6, 4, 6, 5, // (south (+z))
 * 3, 0, 1, 3, 1, 2, // (north (-z))
 * 7, 3, 2, 7, 2, 6, // (east  (+x))
 * 0, 4, 5, 0, 5, 1, // (west  (-x))
 * 2, 1, 5, 2, 5, 6, // (up    (+y))
 * 0, 3, 7, 0, 7, 4  // (down  (-y))
 */

// indices, within each list of 6 cube indices, which represent are the 4
// unique vertices which make up each face
static const uint32 UNIQUE_INDICES[] = {0, 1, 2, 5};

// indices into emitted vertices which make up the two faces for a cube face
static const uint32 FACE_INDICES[] = {0, 1, 2, 0, 2, 3};

static const uint32 CUBE_INDICES[] = {
    4, 7, 6, 4, 6, 5, // (south (+z))
    3, 0, 1, 3, 1, 2, // (north (-z))
    7, 3, 2, 7, 2, 6, // (east  (+x))
    0, 4, 5, 0, 5, 1, // (west  (-x))
    2, 1, 5, 2, 5, 6, // (up    (+y))
    0, 3, 7, 0, 7, 4  // (down  (-y))
};

static const glm::vec3 CUBE_VERTICES[] = {
    glm::vec3(0, 0, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(1, 1, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 1, 1),
    glm::vec3(1, 1, 1),
    glm::vec3(1, 0, 1)
};

static const glm::vec3 CUBE_NORMALS[] = {
    glm::vec3( 0,  0,  1),
    glm::vec3( 0,  0, -1),
    glm::vec3( 1,  0,  0),
    glm::vec3(-1,  0,  0),
    glm::vec3( 0,  1,  0),
    glm::vec3( 0, -1,  0),
};

static const glm::vec2 CUBE_UVS[] = {
    glm::vec2(0, 0),
    glm::vec2(1, 0),
    glm::vec2(1, 1),
    glm::vec2(0, 1),
};

typedef struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
} Vertex;

// CHUNK
// ---------------------------------
typedef struct Chunk {
    uint8 blocks[CHUNK_BLOCK_COUNT];

    struct {
        std::vector<Vertex> vertices;
        std::vector<uint32> indices;

        uint32 vao, vbo, ibo;
    } mesh;

    bool remesh;

    glm::vec2 chunk_position;
    glm::vec2 world_position;
} Chunk;

void chunk_init(Chunk* chunk, glm::vec2 chunk_position);
void chunk_prepare_mesh(Chunk* chunk);
void chunk_finish_mesh(Chunk* chunk);
void chunk_generate_mesh(Chunk* chunk);
void chunk_render(Chunk* chunk);
void chunk_set_position(Chunk* chunk, glm::vec2 new_position);


// WORLD
// ---------------------------------
typedef struct World {
    // Chunk chunks[WORLD_CHUNK_COUNT];

    std::vector<Chunk> chunks;

    glm::vec2 center_chunk;
    glm::vec2 origin_chunk;

    struct {
        uint32 max;
        uint32 count;
    } throttle;
} World;


void world_init(World* world);
void world_set_center(World* world, glm::vec3 camera_position);
void world_render(World* world);
void world_generation(World* world);
void world_update(World* world);

inline glm::vec3 get_block_from_camera_position(glm::vec3 camera_position);

#endif // MYCRAFT_CHUNK_HPP
