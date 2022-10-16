#include "common.hpp"
#include "mycraft.hpp"
#include "vertex_data.hpp"

// Global Variables
GameState game_state;
global float delta_time = 0.0f;
global float last_frame = 0.0f;
global bool32 wireframe_mode = 0;

// Internal function helper
internal void resize_window_callback(GLFWwindow* handle, int w, int h);
internal void cursor_position_callback(GLFWwindow* handle, double x, double y);
internal void process_input(GLFWwindow* handle);
internal void mouse_callback(GLFWwindow* handle, int button, int action, int mods);
internal void key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods);

int
main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    GLFWwindow* window = glfwCreateWindow((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, "MyCraft", 0, 0);

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, resize_window_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            fprintf(stderr, "Failed to inititialize GLAD\n");
            exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_MULTISAMPLE);
    // glCullFace(GL_FRONT);

    glfwSwapInterval(1);

    gamestate_init(game_state);

    while(!glfwWindowShouldClose(window)) {

        glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);

        // TIMING
        float current_frame = (float) glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.2f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

        game_update_and_render(game_state);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return(0);
}

internal void
resize_window_callback(GLFWwindow* handle, int w, int h)
{
    glViewport(0, 0, w, h);
}

internal void
cursor_position_callback(GLFWwindow* handle, double x, double y)
{
    if(game_state.camera.firstmouse) {
        game_state.camera.lastx = (float)x;
        game_state.camera.lasty = (float)y;
        game_state.camera.firstmouse = false;
    }
    float xoff = (float) x - game_state.camera.lastx;
    float yoff = (float) y - game_state.camera.lasty;
    game_state.camera.lastx = (float) x;
    game_state.camera.lasty = (float) y;
    game_state.camera.process_cursor(xoff, yoff);
}

internal void
process_input(GLFWwindow* handle)
{
    if(glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(handle, 1);
    if(glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::FORWARD, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::BACKWARD, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::LEFT, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::RIGHT, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::DOWN, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS)
        game_state.camera.process_keyboard(Direction::UP, delta_time);
}

internal void
mouse_callback(GLFWwindow* handle, int button, int action, int mods)
{
    // TODO(fonsi): do something with mouse buttons
}

internal void
key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_TAB && action == GLFW_PRESS)
        wireframe_mode = !wireframe_mode;
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        game_state.camera.speedup = !game_state.camera.speedup;
}

void
gamestate_init(GameState& game_state)
{
    game_state.renderer.init();
    game_state.camera.init();
    game_state.world.init();
}

void
game_update_and_render(GameState& game_state)
{
    game_state.world.render();
}
