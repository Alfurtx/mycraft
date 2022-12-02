#ifndef MYCRAFT_HPP
#define MYCRAFT_HPP

#include "common.hpp"
#include "mc_chunk.hpp"
#include "mc_camera.hpp"
#include "mc_renderer.hpp"

typedef struct GameState
{
    Renderer renderer;
    Camera   camera;
    World    world;

} GameState;

extern GameState game_state;

void gamestate_init(GameState& game_state);
void gamestate_render(GameState& game_state);

#endif
