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
    UP,      // y+
    DOWN,    // y-
    RIGHT,   // x+
    LEFT,    // x-
    FORWARD, // z-
    BACKWARD // z+
};

global const float32 SCREEN_WIDTH = 1920;
global const float32 SCREEN_HEIGHT = 1080;
global const int32 CHUNK_WIDTH = 32;

#endif
