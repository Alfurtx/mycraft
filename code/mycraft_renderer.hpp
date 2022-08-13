#if !defined(MYCRAFT_RENDERER_H)
#define MYCRAFT_RENDERER_H

#include "common.hpp"

enum ShaderTarget {
    CUBE = 0,
    LAST
};

struct Shader {
    uint program_handle;
    void compile_program(char* vertex_filepath, char* fragment_filepath);
    void use_program();
};

struct Renderer {
    Shader shaders[ShaderTarget::LAST];
};

#endif
