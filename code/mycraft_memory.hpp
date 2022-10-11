#ifndef MYCRAFT_MEMORY_HPP
#define MYCRAFT_MEMORY_HPP

#include "common.hpp"

// TODO(fonsi): make memory arenas grow dynamically

typedef struct Arena {
    uint8* memory;
    uint64 capacity;
    uint64 alloc_position;
} Arena;

Arena* arena_alloc(uint64 size);
void   arena_release(Arena* arena);
void*  arena_push(Arena* arena, uint64 size);
void*  arena_push_zero(Arena* arena, uint64 size);
void   arena_pop(Arena* arena, uint64 size);
uint64 arena_get_pos(Arena* arena);
void   arena_clear(Arena* arena);

#define push_array(arena, type, count) (type*)arena_push((arena), sizeof(type)*(count))
#define push_array_zero(arena, type, count) (type*)arena_push_zero((arena), sizeof(type)*(count))
#define push_struct(arena, type) push_array((arena), (type), 1)
#define push_struct_zero(arena, type) push_array_zero((arena), (type), 1)

// #define ARENA_BASE_SIZE (megabytes(2))

#endif
