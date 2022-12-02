#include "mc_blocks.hpp"

Block BLOCKS[BLOCK_TYPE::COUNT];

void air_init() { BLOCKS[BLOCK_TYPE::AIR].texture_offset = NULL; }
void
grass_init()
{
    BLOCKS[BLOCK_TYPE::GRASS] = {
        [](Direction dir) -> glm::ivec2
        {
            switch (dir) {
            case Direction::UP:
                return glm::ivec2(0, 0);
            case Direction::DOWN:
                return glm::ivec2(2, 0);
            default:
                return glm::ivec2(1, 0);
            }
        }
    };
}

void
dirt_init()
{
    BLOCKS[BLOCK_TYPE::DIRT] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(2, 0);
        }
    };
}
void
water_init()
{
    BLOCKS[BLOCK_TYPE::WATER] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(0, 15);
        }
    };
}
void
iron_init()
{
    BLOCKS[BLOCK_TYPE::IRON] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(0, 4);
        }
    };
}
void
stone_init()
{
    BLOCKS[BLOCK_TYPE::STONE] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(3, 0);
        }
    };
}
void
log_init()
{
    BLOCKS[BLOCK_TYPE::LOG] = {
        [](Direction dir) -> glm::ivec2
        {
            switch (dir) {
            case Direction::UP:
            case Direction::DOWN:
                return glm::ivec2(3, 1);
            default:
                return glm::ivec2(2, 1);
            }
            return glm::ivec2(0, 3);
        }
    };
}
void
sand_init()
{
    BLOCKS[BLOCK_TYPE::SAND] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(0, 1);
        }
    };
}
void
glass_init()
{
    BLOCKS[BLOCK_TYPE::GLASS] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(1, 1);
        }
    };
}
void
wood_init()
{
    BLOCKS[BLOCK_TYPE::WOOD] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(6, 1);
        }
    };
}
void
cobblestone_init()
{
    BLOCKS[BLOCK_TYPE::COBBLESTONE] = {
        [](Direction dir) -> glm::ivec2
        {
            return glm::ivec2(2, 2);
        }
    };
}
