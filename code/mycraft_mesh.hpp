#ifndef MYCRAFT_MESH_HPP
#define MYCRAFT_MESH_HPP

#include "common.hpp"

struct DataBuffer {
    void*  data;
    uint   index;
    uint   capacity;
    uint   count;
};

struct Mesh {
    DataBuffer data;
    DataBuffer indices;
    uint32     vao,
               vbo,
               ibo;

    void init();
    void destroy();
    void prepare();
    void finalize();
};

#endif
