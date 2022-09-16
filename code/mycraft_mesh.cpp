#include "mycraft_mesh.hpp"

// NOTE(fonsi): vertex format [3 float position, 2 float texture, 3 float normal]
#define DATA_BUFFER_SIZE (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * 8 * 8 * sizeof(float))
#define INDICES_BUFFER_SIZE (CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH * 36 * sizeof(uint))

const float VERTICES[] = {
    0, 0, 0, // 0
    1, 0, 0, // 1
    1, 1, 0, // 2
    0, 1, 0, // 3
    0, 0, 1, // 4
    1, 0, 1, // 5
    1, 1, 1, // 6
    0, 1, 1  // 7
};

const uint INDICES[] = {
    1, 0, 3, 1, 3, 2, // north (-z)
    4, 5, 6, 4, 6, 7, // south (+z)
    5, 1, 2, 5, 2, 6, // east (+x)
    0, 4, 7, 0, 7, 3, // west (-x)
    2, 3, 7, 2, 7, 6, // top (+y)
    5, 4, 0, 5, 0, 1, // bottom (-y)
};

struct {
    float data[DATA_BUFFER_SIZE];
    uint  indices[DATA_BUFFER_SIZE];
} GLOBAL_BUFFERS;

void
Mesh::init()
{
    data = GLOBAL_BUFFERS.data;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void
Mesh::destroy()
{
}

void
Mesh::prepare()
{
}

void
Mesh::finalize()
{
}
