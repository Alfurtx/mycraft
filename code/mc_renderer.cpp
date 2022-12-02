#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mc_renderer.hpp"


void
Renderer::init()
{
    world_shader.compile_program((char*)"w:\\mycraft\\shaders\\cube.vert",
                                 (char*)"w:\\mycraft\\shaders\\cube.frag",
                                 (char*) 0);
    atlas.create("w:\\mycraft\\res\\blocks.png");
    atlas.bind();
}

void
Shader::compile_program(char* vertex_filepath, char* fragment_filepath, char* geometry_filepath)
{
    assert(vertex_filepath && fragment_filepath, (char*)"File path is null");

    FILE* fp;
    fopen_s(&fp, vertex_filepath, "rb");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* vertsrc = (char*) _malloca((size + 1));
    fread(vertsrc, 1, size, fp);
    vertsrc[size] = '\0';
    fclose(fp);

    fopen_s(&fp, fragment_filepath, "rb");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* fragsrc = (char*) _malloca((size + 1));
    fread(fragsrc, 1, size, fp);
    fragsrc[size] = '\0';
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

    uint geo;
    char* geosrc;
    if(geometry_filepath) {
        fopen_s(&fp, fragment_filepath, "rb");
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        geosrc = (char*) _malloca((size + 1));
        fread(geosrc, 1, size, fp);
        geosrc[size] = '\0';
        fclose(fp);
        geo = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geo, 1, &geosrc, 0);
        glCompileShader(geo);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            fprintf(stderr, "[ERROR][FRAGMENT COMPILATION]\n%s\n", infoLog);
        }
        glAttachShader(this->program_handle, geo);
    }

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
    if(geometry_filepath) {
        glDeleteShader(geo);
        _freea(geosrc);
    }
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

void
Texture::create(const char* filepath)
{
    texture_type = GL_TEXTURE_2D;
    glGenTextures(1, &handle);
    int width, height, num_components;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filepath, &width, &height, &num_components, 0);
    if (data)
    {
        GLenum format;
        switch (num_components)
        {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        };

        glBindTexture(texture_type, handle);
        glTexImage2D(texture_type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(texture_type);

        glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
        } else {
        fprintf(stderr, "Failed to load texture path\n");
        stbi_image_free(data);
    }
}

void
Texture::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(texture_type, handle);
}

void
Texture::destroy()
{
    glDeleteTextures(1, &handle);
}
