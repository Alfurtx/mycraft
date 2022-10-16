#include "mycraft_chunk.hpp"
#include "mycraft.hpp"

#define for_each_chunk(_i, _j) \
    for(int32 _i = -(int32)trunc(WORLD_CHUNK_WIDTH / 2); _i < (int32)trunc(WORLD_CHUNK_WIDTH / 2); _i++) \
        for(int32 _j = -(int32)trunc(WORLD_CHUNK_WIDTH / 2); _j < (int32)trunc(WORLD_CHUNK_WIDTH / 2); _j++) \

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
    int32 min = (int32)trunc(-WORLD_CHUNK_WIDTH / 2);
    return((i - min) + (j - min) * WORLD_CHUNK_WIDTH);
}
internal inline uint32
chunk_coords_to_pos(glm::vec2 v)
{
    glm::vec2 c = game_state.world.center_chunk;
    int32 mini = (int32)trunc(-WORLD_CHUNK_WIDTH / 2) - (int32)c.x;
    int32 minj = (int32)trunc(-WORLD_CHUNK_WIDTH / 2) - (int32)c.y;
    int32 i = (int32)v[0];
    int32 j = (int32)v[1];
    return((i - mini) + (j - minj) * WORLD_CHUNK_WIDTH);
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
    int32 border = (int32)trunc(WORLD_CHUNK_WIDTH / 2);
    glm::vec2 c = game_state.world.center_chunk;
    return(pos.x >= -border - c.x  &&
           pos.x <=  border - c.x  &&
           pos.y >= -border - c.y  &&
           pos.y <=  border - c.y );
}
internal inline uint32
get_block_in_world(glm::vec3 bpos, glm::vec3 rbpos)
{
    bool oof = bpos[1] >= CHUNK_WIDTH || bpos[1] < 0;
    if(oof) return(0);

    glm::vec2 cpos = glm::vec2(floor(bpos.x / CHUNK_WIDTH),
                               floor(bpos.z / CHUNK_WIDTH));

    if(is_chunk_in_world(cpos)) {
        for(uint32 i = 0; i < 3; i++) {
            if(rbpos[i] == -1) rbpos[i] = CHUNK_WIDTH - 1;
            if(rbpos[i] == CHUNK_WIDTH) rbpos[i] = 0;
        }

        Chunk& c = game_state.world.chunks[chunk_coords_to_pos(cpos)];
        uint32 barrpos = block_coords_to_pos((uint32)rbpos.x,
                                             (uint32)rbpos.y,
                                             (uint32)rbpos.z);
        uint32 b = c.blocks[barrpos];
        return(b);
    }

    return(0);
}

void
Chunk::init(glm::vec2 cpos)
{
    chunk_position = cpos;
    world_position = cpos * (float32)(CHUNK_WIDTH);
    remesh = true;
    chunkmesh.init();
    for_each_block(i, j, k) {
        if(j >= 0 && j <= 5) blocks[block_coords_to_pos(glm::vec3(i, j, k))] = BLOCK_TYPE::GRASS;
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
            for(uint32 m = 0; m < 6; m++) {
                glm::vec3 dirvec = bpos + DIRVECS[m];
                glm::vec3 wbpos = glm::vec3(bpos[0] + world_position[0],
                                            bpos[1],
                                            bpos[2] + world_position[1]);
                glm::vec3 wdirvec = wbpos + DIRVECS[m];

                uint32 neighbor, testneighbor;
                testneighbor = get_block_in_world(wdirvec, dirvec);
                neighbor = testneighbor;
                // if(is_pos_in_chunk(dirvec)) {
                //     neighbor = blocks[block_coords_to_pos(dirvec)];
                //     if(testneighbor != neighbor) {
                //         testneighbor = get_block_in_world(wdirvec, dirvec);
                //         neighbor = blocks[block_coords_to_pos(dirvec)];
                //     }
                // } else {
                //     neighbor = get_block_in_world(wdirvec, dirvec);
                // }

                if(neighbor == 0) {
                    chunkmesh.add_face((Direction) m, bpos);
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
    center_chunk = glm::vec2(0);
    for_each_chunk(i, j) {
        chunks[chunk_coords_to_pos(i, j)].init(glm::vec2(i, j));
    }
}

void
World::render()
{
    for_each_chunk(i, j) {
        Chunk& c = chunks[chunk_coords_to_pos(i, j)];
        if(c.remesh) { c.generate_mesh(); c.remesh = false; }
        c.render();
    }
}
