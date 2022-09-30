#ifndef MYCRAFT_MESH_HPP
#define MYCRAFT_MESH_HPP

#include "common.hpp"

struct Mesh {
    float32* vertices;
    uint32*  indices;

    void add_face(Direction direction);
};

#endif
