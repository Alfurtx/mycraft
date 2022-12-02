#ifndef BLOCKS_HPP
#define BLOCKS_HPP

#include "common.hpp"

struct Block {
    glm::ivec2 (*texture_offset)(Direction dir);
};

extern Block BLOCKS[BLOCK_TYPE::COUNT];

extern void air_init();
extern void grass_init();
extern void dirt_init();
extern void water_init();
extern void iron_init();
extern void stone_init();
extern void log_init();
extern void sand_init();
extern void glass_init();
extern void wood_init();
extern void cobblestone_init();

internal inline void
block_init()
{
    air_init();
    grass_init();
    dirt_init();
    water_init();
    iron_init();
    stone_init();
    log_init();
    sand_init();
    glass_init();
    wood_init();
    cobblestone_init();
}

#endif
