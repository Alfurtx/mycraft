#include "mycraft_mesh.hpp"

#define VERTICES_BUFFER_SIZE (32 * 32 * 32 * * 8 * (3 + 2 + 3))
#define INDICES_BUFFER_SIZE (32 * 32 * 32 * (12 * 3))

const uint32 UNIQUE_INDICES = { 0, 1, 2, 5 };

const float32 VERTICES[] = {
    0, 0, 0, // 0
    1, 0, 0, // 1
    1, 1, 0, // 2
    0, 1, 0, // 3
    0, 0, 1, // 4
    1, 0, 1, // 5
    1, 1, 1, // 6
    0, 1, 1  // 7
};

const uint32 INDICES[] = {
    1, 0, 3, 1, 3, 2, // north (-z)
    4, 5, 6, 4, 6, 7, // south (+z)
    5, 1, 2, 5, 2, 6, // east (+x)
    0, 4, 7, 0, 7, 3, // west (-x)
    2, 3, 7, 2, 7, 6, // top (+y)
    5, 4, 0, 5, 0, 1, // bottom (-y)
};

void
Mesh::add_face(Direction direction)
{
}
