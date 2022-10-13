#ifndef MYCRAFT_HPP
#define MYCRAFT_HPP

#include "common.hpp"
#include "mycraft_chunk.hpp"
#include "mycraft_camera.hpp"
#include "mycraft_renderer.hpp"

typedef struct GameState
{
    Renderer renderer;
    Camera   camera;
    World    world;

} GameState;

extern GameState game_state;

void gamestate_init(GameState& game_state);
void game_update_and_render(GameState& game_state);

#endif