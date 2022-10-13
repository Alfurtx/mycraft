#ifndef MYCRAFT_MESH_HPP
#define MYCRAFT_MESH_HPP

#include "common.hpp"

typedef struct MeshDataBuffer {
    void*  data;
    uint32 count;
    uint32 index;
} MeshDataBuffer;

struct Mesh {
    MeshDataBuffer vertices;
    MeshDataBuffer indices;
    uint32 vertex_count;

    uint32 vao;
    uint32 vbo;
    uint32 ibo;

    void init();
    void prepare();
    void finalize();
    void add_face(Direction dir, glm::vec3 block_position);
};

#endif
