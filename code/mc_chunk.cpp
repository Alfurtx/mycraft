#include "mc_chunk.hpp"
#include "mycraft.hpp"

extern "C" {
#include "noise1234.h"
}

inline glm::vec3
get_block_from_camera_position(glm::vec3 camera_position)
{
    glm::vec3 result;
    result.x = camera_position.x;

    if(camera_position.y < 0) result.y = 0;
    else if(camera_position.y > CHUNK_WIDTH_Y) result.y = CHUNK_WIDTH_Y;
    else result.y = camera_position.y;

    result.z = camera_position.z;

    return(result);
}

// NOTE(fonsi): min and max are inclusive
INTERNAL bool
in_range(float val, float min, float max)
{
    return(val >= min && val < max);
}

INTERNAL glm::vec3
chunk_get_block_coords_from_arrpos(int32 position)
{
    glm::vec3 result;
    result.z = (float)(position / (CHUNK_WIDTH_X * CHUNK_WIDTH_Y));
    result.y = (float)((position / CHUNK_WIDTH_X) % CHUNK_WIDTH_Y);
    result.x = (float)(position % CHUNK_WIDTH_X);
    return(result);
}

INTERNAL int32
chunk_get_block_arrpos_from_coords(glm::vec3 coords)
{
    return((int32)coords.x +
           (int32)coords.y * CHUNK_WIDTH_X +
           (int32)coords.z * CHUNK_WIDTH_Y * CHUNK_WIDTH_X);
}

INTERNAL bool
chunk_check_block_exists(Chunk* chunk, int32 array_position)
{
    return(chunk->blocks[array_position] != 0);
}
INTERNAL bool
chunk_check_block_exists(Chunk* chunk, glm::vec3 coords_position)
{
    // NOTE(fonsi): only checks coords in chunk local space (0..CHUNK_WIDTH)
    // be careful with world space (allows all values)
    for(int32 i = 0; i < 3; i++) {
        if(coords_position[i] < 0) return(false);
        if(i == 0 && coords_position[i] >= CHUNK_WIDTH_X) return(false);
        if(i == 1 && coords_position[i] >= CHUNK_WIDTH_Y) return(false);
        if(i == 2 && coords_position[i] >= CHUNK_WIDTH_Z) return(false);

        // if(coords_position[i] < 0 || coords_position[i] >= CHUNK_WIDTH)
        // {
        //     return(false);
        // }
    }

    int32 arrpos = chunk_get_block_arrpos_from_coords(coords_position);
    return(chunk->blocks[arrpos] != 0);
}

INTERNAL bool world_check_block_exists(World* world, glm::vec3 position);

void
chunk_init(Chunk* chunk, glm::vec2 chunk_position)
{
    chunk->chunk_position = chunk_position;
    chunk->world_position = { chunk_position.x * CHUNK_WIDTH_X, chunk_position.y * CHUNK_WIDTH_Z };

    // Fill out all blocks at heigh level 1 as grass blocks
    for(int32 pos = 0; pos < CHUNK_BLOCK_COUNT; pos++)
    {
        int32 yposition = (pos / CHUNK_WIDTH_X) % CHUNK_WIDTH_Y;
        if(in_range(yposition, 1, 4)) chunk->blocks[pos] = BLOCK_TYPE::LOG;
        // chunk->blocks[pos] = BLOCK_TYPE::WOOD;
    }

    // initialize mesh data
    chunk->mesh = {};
    glGenVertexArrays(1, &chunk->mesh.vao);
    glGenBuffers(1, &chunk->mesh.vbo);
    glGenBuffers(1, &chunk->mesh.ibo);
    glBindVertexArray(chunk->mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.ibo);

    // set flags
    chunk->remesh = true;
}

INTERNAL void
emit_face(Chunk* chunk,
          std::vector<Vertex> &vertices,
          std::vector<uint32> &indices,
          glm::vec3 position,
          glm::vec2 uv_offset,
          glm::vec2 uv_size,
          Direction direction)
{
    const uint32 offset = (uint32) vertices.size();
    for(uint32 i = 0; i < 4; i++)
    {
        Vertex v;
        v.position = position + CUBE_VERTICES[CUBE_INDICES[(direction * 6) + UNIQUE_INDICES[i]]];
        v.normal = CUBE_NORMALS[direction];
        // TODO(fonsi): add texture to mesh generation
        v.texture = (CUBE_UVS[i] * uv_size) + uv_offset;
        vertices.push_back(v);
    }

    for(uint32 i : FACE_INDICES)
    {
        indices.push_back(offset + i);
    }
}

void
chunk_generate_mesh(Chunk* chunk)
{
    for(uint32 i = 0; i < CHUNK_BLOCK_COUNT; i++)
    {
        if(chunk->blocks[i]) {

            for(uint32 face_direction = 0;
                face_direction < 6;
                face_direction++)
            {
                glm::vec3 bpos = chunk_get_block_coords_from_arrpos(i);
                glm::vec3 dirvec = DIRVECS[face_direction];
                glm::vec3 npos = bpos + dirvec;

                glm::vec3 wbpos = bpos + glm::vec3(chunk->world_position.x, 0, chunk->world_position.y);
                glm::vec3 wnpos = wbpos + dirvec;

                glm::vec2 uv_unit = glm::vec2(1.0f) / glm::vec2(16.0f);
                Direction d = (Direction)face_direction;
                glm::vec2 uv_offset = BLOCKS[chunk->blocks[i]].texture_offset(d);

                // if(!chunk_check_block_exists(chunk, npos))
                if(!world_check_block_exists(&game_state.world, wnpos))
                {
                    emit_face(chunk,
                            chunk->mesh.vertices,
                            chunk->mesh.indices,
                            bpos,
                            glm::vec2(uv_offset.x, 16 - uv_offset.y - 1) * uv_unit,
                            uv_unit,
                            (Direction)face_direction);
                }

            }

        }
    }
}

void
chunk_prepare_mesh(Chunk* chunk)
{
    chunk->mesh.vertices.clear();
    chunk->mesh.indices.clear();
}

void
chunk_finish_mesh(Chunk* chunk)
{
    // bind corresponding buffers
    glBindVertexArray(chunk->mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.ibo);

    // fill those buffers with the mesh data
    size_t vertex_data_size = chunk->mesh.vertices.size() * 8 * sizeof(float);
    float* vertex_data = &chunk->mesh.vertices[0].position.x;
    size_t indices_data_size = chunk->mesh.indices.size() * sizeof(uint32);
    uint32* indices_data = &chunk->mesh.indices[0];

    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_data_size, indices_data, GL_STATIC_DRAW);

    // tell opengl how that data is structured
    glBindVertexArray(chunk->mesh.vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // IMPORTANT! set to not remesh so that the data can be reused
    chunk->remesh = false;
}

void
chunk_render(Chunk* chunk)
{
    glm::vec3 transvec = glm::vec3(chunk->world_position[0],
                                   0,
                                   chunk->world_position[1]);

    Shader& shader = game_state.renderer.world_shader;
    shader.use();
    shader.set_uniform((char*)"projection",
                       game_state.camera.get_projection());
    shader.set_uniform((char*)"view", game_state.camera.get_view());
    shader.set_uniform((char*)"model", glm::translate(glm::mat4(1.0f), transvec));
    glBindVertexArray(chunk->mesh.vao);
    glDrawElements(GL_TRIANGLES, (uint32)chunk->mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void
chunk_set_position(Chunk* chunk, glm::vec2 new_position)
{
    chunk->chunk_position = new_position;
    chunk->world_position = { new_position.x * CHUNK_WIDTH_X, new_position.y * CHUNK_WIDTH_Z };
}


INTERNAL glm::vec2
world_chunk_coords_from_arrpos(World* world, uint32 position)
{
    int32 pos  = (int32)position;
    int32 xmin = (int32)world->origin_chunk.x;
    int32 ymin = (int32)world->origin_chunk.y;

    glm::vec2 result = glm::vec2(0);
    result.y = (int32)(trunc(pos / WORLD_CHUNK_WIDTH)) + ymin;
    result.x = (int32)(trunc(pos % WORLD_CHUNK_WIDTH)) + xmin;

    return(result);
}

INTERNAL uint32
world_chunk_arrpos_from_coords(World* world, glm::vec2 coords)
{
    int32 xmin = (int32)world->origin_chunk.x;
    int32 ymin = (int32)world->origin_chunk.y;
    return((uint32) ((coords.y - ymin) * WORLD_CHUNK_WIDTH + (coords.x - xmin)));
}

INTERNAL bool
world_check_block_exists(World* world, glm::vec3 position)
{
    // check block is IN the world
    glm::vec2 last_chunk_pos = world_chunk_coords_from_arrpos(world, WORLD_CHUNK_COUNT-1);
    if( !in_range(position.x, world->origin_chunk.x * CHUNK_WIDTH_X, last_chunk_pos.x * CHUNK_WIDTH_X + CHUNK_WIDTH_X) ||
        !in_range(position.z, world->origin_chunk.y * CHUNK_WIDTH_Z, last_chunk_pos.y * CHUNK_WIDTH_Z + CHUNK_WIDTH_Z) ||
        !in_range(position.y, 0, CHUNK_WIDTH_Y))
    {
        return(false);
    }

    // get the chunk it belongs to
    glm::vec2 chunk_coords = glm::vec2(floor(position.x / CHUNK_WIDTH_X), floor(position.z / CHUNK_WIDTH_Z));
    glm::vec2 chunk_wcoords = glm::vec2(chunk_coords.x * CHUNK_WIDTH_X, chunk_coords.y * CHUNK_WIDTH_Z);
    glm::vec3 block_local_pos = glm::vec3(position.x - chunk_wcoords.x, position.y, position.z - chunk_wcoords.y);
    uint32 carrpos = world_chunk_arrpos_from_coords(world, chunk_coords);
    Chunk* chunk = &world->chunks[carrpos];

    // get the block
    // uint32 block = chunk_get_block_arrpos_from_coords(block_local_pos);
    return(chunk_check_block_exists(chunk, block_local_pos));
}

void
world_init(World* world)
{
    world->center_chunk = glm::vec2(0);
    world->origin_chunk = glm::vec2(-1, -1);

    // test std::vector for chunks
    world->chunks.resize(WORLD_CHUNK_COUNT);

    for(uint32 i = 0; i < WORLD_CHUNK_COUNT; i++)
    {
        glm::vec2 cpos = world_chunk_coords_from_arrpos(world, i);
        chunk_init(&world->chunks[i], cpos);
    }

    world->throttle.max = 13;
    world->throttle.count = 0;

    // test
    // world->throttle = {8, 0};
}

void
world_set_center(World* world, glm::vec3 camera_position)
{
    // return;
    glm::vec3 player_position = get_block_from_camera_position(camera_position);

    // check if player is in center chunk
    float minx = world->center_chunk.x * CHUNK_WIDTH_X;
    float miny = world->center_chunk.y * CHUNK_WIDTH_Z;
    float maxx = world->center_chunk.x * CHUNK_WIDTH_X + CHUNK_WIDTH_X;
    float maxy = world->center_chunk.y * CHUNK_WIDTH_Z + CHUNK_WIDTH_Z;

    if(in_range(player_position.x, minx, maxx) &&
       in_range(player_position.z, miny, maxy))
        return;

    // change center chunk
    glm::vec2 chunk_coords = glm::vec2(floor(player_position.x / CHUNK_WIDTH_X),
                                       floor(player_position.z / CHUNK_WIDTH_Z));
    world->center_chunk = chunk_coords;

    // change origin chunk
    glm::vec2 offset = glm::vec2(trunc(WORLD_CHUNK_WIDTH / 2),
                                 trunc(WORLD_CHUNK_WIDTH / 2));
    world->origin_chunk = world->center_chunk - offset;

    // flag for remesh and reset positions for chunks

    std::vector<Chunk> old(world->chunks);

    world->chunks.clear();
    world->chunks.resize(WORLD_CHUNK_COUNT);

    for(uint32 i = 0; i < WORLD_CHUNK_COUNT; i++)
    {
        glm::vec2 cpos = world_chunk_coords_from_arrpos(world, i);
        int32 reinit = -1;
        for(uint32 i = 0; i < WORLD_CHUNK_COUNT; i++)
            if(old[i].chunk_position == cpos) {
                reinit = i;
                break;
            }
        if(reinit == -1)
            chunk_init(&world->chunks[i], cpos);
        else {
            world->chunks.at(i) = old[reinit];
            world->chunks[i].remesh = true;
        }
    }
}

void
world_render(World* world)
{
    for(uint32 i = 0; i < WORLD_CHUNK_COUNT; i++)
    {
        if(world->chunks[i].remesh &&
           world->throttle.count < world->throttle.max)
        {
            chunk_prepare_mesh(&world->chunks[i]);
            chunk_generate_mesh(&world->chunks[i]);
            chunk_finish_mesh(&world->chunks[i]);
            world->throttle.count++;
        }

        chunk_render(&world->chunks[i]);
    }
}

void
world_generation(World* world)
{
    float ej = noise3(1, 2, 3);
}

void
world_update(World* world)
{
    world->throttle.count = 0;
}
