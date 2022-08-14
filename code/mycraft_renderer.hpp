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
    void use();

    void set_uniform(char* name, glm::mat4 value);
    void set_uniform(char* name, glm::mat3 value);
    void set_uniform(char* name, glm::vec4 value);
    void set_uniform(char* name, glm::vec3 value);
    void set_uniform(char* name, glm::vec2 value);
    void set_uniform(char* name, int value);
    void set_uniform(char* name, uint value);
};

struct Renderer {
    Shader shaders[ShaderTarget::LAST];
};

#endif
