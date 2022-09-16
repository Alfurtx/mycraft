#include "mycraft_renderer.hpp"

void
Shader::compile_program(char* vertex_filepath, char* fragment_filepath)
{
    assert(vertex_filepath && fragment_filepath, (char*)"File path is null");

    FILE* fp;
    fopen_s(&fp, vertex_filepath, "rb");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* vertsrc = (char*) _malloca((size + 1));
    fread(vertsrc, 1, size, fp);
    fclose(fp);

    fopen_s(&fp, fragment_filepath, "rb");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* fragsrc = (char*) _malloca((size + 1));
    fread(fragsrc, 1, size, fp);
    fclose(fp);

    uint vert, frag;
    int  success;
    char infoLog[512];

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertsrc, 0);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR][VERTEX COMPILATION]\n%s\n", infoLog);
    }

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragsrc, 0);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR][FRAGMENT COMPILATION]\n%s\n", infoLog);
    }

    this->program_handle = glCreateProgram();
    glAttachShader(this->program_handle, vert);
    glAttachShader(this->program_handle, frag);
    glLinkProgram(this->program_handle);
    glGetProgramiv(this->program_handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(this->program_handle, 512, NULL, infoLog);
        fprintf(stderr, "[ERROR][PROGRAM LINKING]\n%s\n", infoLog);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    _freea(vertsrc);
    _freea(fragsrc);
}

void
Shader::use()
{
    glUseProgram(this->program_handle);
}

void
Shader::set_uniform(char* name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program_handle, name),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
}

void
Shader::set_uniform(char* name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(program_handle, name),
                       1,
                       GL_FALSE,
                       glm::value_ptr(value));
}

void
Shader::set_uniform(char* name, glm::vec4 value)
{
    glUniform4fv(glGetUniformLocation(program_handle, name),
                       1,
                       glm::value_ptr(value));
}

void
Shader::set_uniform(char* name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(program_handle, name),
                       1,
                       glm::value_ptr(value));
}

void
Shader::set_uniform(char* name, glm::vec2 value)
{
    glUniform2fv(glGetUniformLocation(program_handle, name),
                       1,
                       glm::value_ptr(value));
}

void
Shader::set_uniform(char* name, int value)
{
    glUniform1i(glGetUniformLocation(program_handle, name),
                       value);
}

void
Shader::set_uniform(char* name, uint value)
{
    glUniform1ui(glGetUniformLocation(program_handle, name),
                       value);
}
