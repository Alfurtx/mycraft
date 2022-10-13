#include "mycraft_chunk.hpp"
#include "mycraft.hpp"

#define for_each_chunk(_i, _j) \
    for(int32 _i = -(int32)(WORLD_CHUNK_WIDTH / 2); _i < (int32)WORLD_CHUNK_WIDTH / 2; _i++) \
        for(int32 _j = -(int32)(WORLD_CHUNK_WIDTH / 2); _j < (int32)WORLD_CHUNK_WIDTH / 2; _j++) \

#define for_each_block(_i, _j, _k) \
    for(uint32 _i = 0; _i < CHUNK_WIDTH; _i++) \
        for(uint32 _j = 0; _j < CHUNK_WIDTH; _j++) \
            for(uint32 _k = 0; _k < CHUNK_WIDTH; _k++) \

internal inline uint32
block_coords_to_pos(uint32 i, uint32 j, uint32 k)
{
    return(i + j * CHUNK_WIDTH + k * CHUNK_WIDTH * CHUNK_WIDTH);
}
internal inline uint32
block_coords_to_pos(glm::vec3 pos)
{
    uint32 i = (uint32) pos[0];
    uint32 j = (uint32) pos[1];
    uint32 k = (uint32) pos[2];
    return(i + j * CHUNK_WIDTH + k * CHUNK_WIDTH * CHUNK_WIDTH);
}
internal inline glm::vec3
block_pos_to_coords(uint32 pos)
{
    return(glm::vec3(pos / (CHUNK_WIDTH * CHUNK_WIDTH),
                    (pos / CHUNK_WIDTH) % CHUNK_WIDTH,
                    pos % CHUNK_WIDTH));
}
internal inline uint32
chunk_coords_to_pos(int32 i, int32 j)
{
    int32 min = -WORLD_CHUNK_WIDTH / 2;
    return((i - min) + (j - min) * WORLD_CHUNK_WIDTH);
}
internal inline uint32
chunk_coords_to_pos(glm::vec2 v)
{
    int32 min = -WORLD_CHUNK_WIDTH / 2;
    int32 i = (int32)v[0];
    int32 j = (int32)v[1];
    return((i - min) + (j - min) * WORLD_CHUNK_WIDTH);
}
internal inline bool
is_pos_in_chunk(glm::vec3 pos)
{
    return(pos[0] < CHUNK_WIDTH &&
           pos[1] < CHUNK_WIDTH &&
           pos[2] < CHUNK_WIDTH &&
           pos[0] >= 0 &&
           pos[1] >= 0 &&
           pos[2] >= 0);
}
internal inline bool
is_chunk_in_world(glm::vec2 pos)
{
    int32 border = (int32)(WORLD_CHUNK_WIDTH / 2);
    return(pos[0] >= -border &&
           pos[0] <= border &&
           pos[1] >= -border &&
           pos[1] <= border);
}
internal inline uint32
get_block_in_world(glm::vec3 pos)
{
    glm::vec2 chunk_pos;
    chunk_pos = glm::vec2(pos[0] / CHUNK_WIDTH,
                          pos[2] / CHUNK_WIDTH);
    if(is_chunk_in_world(chunk_pos)) {
        glm::vec2 chunk_world_pos = glm::vec2(chunk_pos[0] * CHUNK_WIDTH,
                                              chunk_pos[1] * CHUNK_WIDTH);
        glm::vec3 relative_block_pos = glm::vec3(pos[0] - chunk_world_pos[0],
                                                 pos[1],
                                                 pos[2] - chunk_world_pos[1]);
        for(int32 i = 0; i < 3; i++ ) relative_block_pos[i] = fabs(relative_block_pos[i]);
        Chunk& c = game_state.world.chunks[chunk_coords_to_pos(chunk_pos)];
        uint32 barrpos = block_coords_to_pos(relative_block_pos);
        uint32 b = c.blocks[barrpos];
        return(b);
    }

    return(0);
}

void
Chunk::init(glm::vec2 cpos)
{
    chunk_position = cpos;
    world_position = glm::vec2(cpos[0] * CHUNK_WIDTH, cpos[1] * CHUNK_WIDTH);
    chunkmesh.init();
    for_each_block(i, j, k) {
        if(j == 0) blocks[block_coords_to_pos(glm::vec3(i, j, k))] = BLOCK_TYPE::GRASS;
        else blocks[block_coords_to_pos(glm::vec3(i, j, k))] = BLOCK_TYPE::AIR;
    }
}

void
Chunk::generate_mesh()
{
    chunkmesh.prepare();
    for_each_block(i, j, k) {
        glm::vec3 bpos = glm::vec3(i, j, k);
        uint32 block = blocks[block_coords_to_pos(bpos)];
        if(block) {
            for(uint32 i = 0; i < 6; i++) {
                glm::vec3 dirvec = bpos + DIRVECS[i];
                glm::vec3 wdirvec = bpos * (float32)CHUNK_WIDTH + DIRVECS[i];
                if(!is_pos_in_chunk(dirvec)) {
                    uint32 neighbor = get_block_in_world(wdirvec);
                    if(neighbor != 0) {
                        chunkmesh.add_face((Direction) i, bpos);
                    }
                } else {
                    uint32 neighbor = blocks[block_coords_to_pos(dirvec)];
                    if(neighbor == 0) {
                        chunkmesh.add_face((Direction) i, bpos);
                    }
                }
            }
        }
    }
    chunkmesh.finalize();
}

void
Chunk::render()
{
    game_state.renderer.world_shader.use();
    game_state.renderer.world_shader.set_uniform((char*)"projection", game_state.camera.get_projection());
    game_state.renderer.world_shader.set_uniform((char*)"view", game_state.camera.get_view());
    game_state.renderer.world_shader.set_uniform((char*)"model", glm::translate(glm::mat4(1.0f),glm::vec3(world_position[0], 0, world_position[1])));
    glBindVertexArray(chunkmesh.vao);
    glDrawElements(GL_TRIANGLES, chunkmesh.indices.count, GL_UNSIGNED_INT, 0);
}

void
World::init()
{
    has_world_changed = true;
    for_each_chunk(i, j) {
        chunks[chunk_coords_to_pos(i, j)].init(glm::vec2(i, j));
    }
}

void
World::render()
{
    if(has_world_changed) {
        for_each_chunk(i, j) {
            chunks[chunk_coords_to_pos(i, j)].generate_mesh();
        }
        has_world_changed = false;
    }

    for_each_chunk(i, j) {
        chunks[chunk_coords_to_pos(i, j)].render();
    }
}
