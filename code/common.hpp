#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// Necessary to avoid complains about using fopen instead of fopen_s
#define _CRT_SECURE_NO_WARNINGS

// NOTE(fonsi): custom assert implementation
#define NDEBUG
#include <cassert>
#undef assert
#define assert(E, M) (void)((E) || (_assert(#E, __FILE__, __LINE__, M),0))
inline void
_assert(const char* expr, const char* filename, const int line, char* message)
{
    if(!message) {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, expr);
    } else {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, message);
    }
    exit(EXIT_FAILURE);
}

#define internal static
#define global static
#define local static

#define kilobytes(num) ((number)*1024ull)
#define megabytes(num) (kilobytes(number)*1024ull)
#define gigabytes(num) (megabytes(number)*1024ull)
#define terabytes(num) (gigabytes(number)*1024ull)

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef float    float32;
typedef double   float64;
typedef int      bool32;

typedef unsigned int uint;

// IMPORTANT(fonsi): The Z+ coordinate is towards the camera when it spawns,
// so FORWARDS IS Z- and BACKWARDS IS Z+
/*
 *    y+
 *    |
 *    | / z-
 *    |/_____ x+
 */
enum Direction {
    FORWARD = 0, // z-
    BACKWARD,    // z+
    RIGHT,       // x+
    LEFT,        // x-
    UP,          // y+
    DOWN,        // y-
};

static glm::vec3 DIRVECS[] = {
    glm::vec3(0 , 0, -1),
    glm::vec3(0 , 0, 1),
    glm::vec3(1 , 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(0 , 1, 0),
    glm::vec3(0 ,-1, 0),
};

global const float32 SCREEN_WIDTH = 1920;
global const float32 SCREEN_HEIGHT = 1080;
global const int32 CHUNK_WIDTH = 32;
global const int32 CHUNK_BLOCK_COUNT = CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH;
global const int32 WORLD_CHUNK_WIDTH = 9;
global const int32 WORLD_CHUNK_COUNT = WORLD_CHUNK_WIDTH * WORLD_CHUNK_WIDTH;

enum BLOCK_TYPE {
    AIR = 0,
    GRASS,
    STONE,
    SAND,
    SNOW
};

#endif
