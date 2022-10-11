#include "mycraft_memory.hpp"

Arena*
arena_alloc(uint64 size)
{
    // TODO(fonsi): include arena.memory allocation in the arena allocation?

    Arena* arena = (Arena*)malloc(sizeof(*arena));
    arena->memory = (uint8*)malloc(sizeof(uint8) * size);
    arena->capacity = size;
    arena->alloc_position = 0;
    return(arena);
}

void
arena_release(Arena* arena)
{
    assert(arena && arena.memory);
    free(arena.memory);
    free(arena);
    arena = 0;
}

void*
arena_push(Arena* arena, uint64 size)
{
    assert(arena->alloc_position + size <= arena->capacity);
    void* res = arena->memory + arena->alloc_position;
    arena->alloc_position += size;
    return(res);
}

void*
arena_push_zero(Arena* arena, uint64 size)
{
    assert(arena->alloc_position + size <= arena->capacity);
    void* res = arena->memory + arena->alloc_position;
    arena->alloc_position += size;
    memset(res, 0, size);
    return(res);
}

void
arena_pop(Arena* arena, uint64 size)
{
    assert(arena && alloc_position >= size)
    arena->alloc_position -= size;
}

uint64
arena_get_pos(Arena* arena)
{
    return(arena->alloc_position);
}

void
arena_clear(Arena* arena)
{
    arena->alloc_position = 0;
}
