#include "mycraft_mesh.hpp"

#define VERTICES_BUFFER_SIZE (32 * 32 * 32 * 8 * (3 + 2 + 3) * sizeof(float32))
#define INDICES_BUFFER_SIZE (32 * 32 * 32 * (12 * 3) * sizeof(float32))
#define APPEND(_type, _buffer, _data)               \
    (((_type*) _buffer.data)[_buffer.index++] = _data)

uint32 UNIQUE_INDICES[] = { 0, 1, 2, 5 };

float32 VERTICES[] = {
    0, 0, 0, // 0
    1, 0, 0, // 1
    1, 1, 0, // 2
    0, 1, 0, // 3
    0, 0, 1, // 4
    1, 0, 1, // 5
    1, 1, 1, // 6
    0, 1, 1  // 7
};

uint32 INDICES[] = {
    1, 0, 3, 1, 3, 2, // north (-z)
    4, 5, 6, 4, 6, 7, // south (+z)
    5, 1, 2, 5, 2, 6, // east (+x)
    0, 4, 7, 0, 7, 3, // west (-x)
    2, 3, 7, 2, 7, 6, // top (+y)
    5, 4, 0, 5, 0, 1, // bottom (-y)
};

struct {
    float32 vertices[VERTICES_BUFFER_SIZE];
    uint32  indices[INDICES_BUFFER_SIZE];
} global_buffers;

void
Mesh::init()
{
    vertices = {};
    indices = {};
    vertex_count = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void
Mesh::prepare()
{
    vertices.data  = global_buffers.vertices;
    vertices.count = 0;
    vertices.index = 0;
    indices.data  = global_buffers.indices;
    indices.count = 0;
    indices.index = 0;
}

void
Mesh::finalize()
{
    vertices.count = vertices.index;
    vertices.index = 0;
    indices.count = indices.index;
    indices.index = 0;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER, vertices.count * sizeof(float32), vertices.data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.count * sizeof(uint32), indices.data, GL_STATIC_DRAW);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float32), (void*)0);
    glEnableVertexAttribArray(0);
}

void
Mesh::add_face(Direction dir, glm::vec3 block_position)
{
    for(uint32 i = 0; i < 4; i++) {
        float32* vertex = &VERTICES[INDICES[dir * 6 + UNIQUE_INDICES[i]] * 3];
        ((float32*) vertices.data)[vertices.index++] = vertex[0] + block_position[0];
        ((float32*) vertices.data)[vertices.index++] = vertex[1] + block_position[1];
        ((float32*) vertices.data)[vertices.index++] = vertex[2] + block_position[2];
        // APPEND(float32, vertices, vertex[0] + block_position[0]);
        // APPEND(float32, vertices, vertex[1] + block_position[1]);
        // APPEND(float32, vertices, vertex[2] + block_position[2]);
    }

    for(uint32 i = 0; i < 6; i++) {
        ((uint32*) indices.data)[indices.index++] = vertex_count + INDICES[i];
        // APPEND(uint32, indices, vertex_count + INDICES[i]);
    }

    vertex_count += 4;
}
