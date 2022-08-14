#include "common.hpp"
#include "mycraft_camera.hpp"
#include "mycraft_renderer.hpp"

internal void resize_window_callback(GLFWwindow* handle, int w, int h);
internal void cursor_position_callback(GLFWwindow* handle, double x, double y);
internal void process_input(GLFWwindow* handle);
internal void mouse_callback(GLFWwindow* handle, int button, int action, int mods);

float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

// CAMERA
Camera camera;

float delta_time = 0.0f;
float last_frame = 0.0f;

int
main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "MyCraft", 0, 0);

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, resize_window_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            fprintf(stderr, "Failed to inititialize GLAD\n");
            exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(1);

    // SHADER SOURCE AND COMPILATION
    //
    Shader shader;
    shader.compile_program((char*)"w:\\mycraft\\shaders\\cube.vert",
                           (char*)"w:\\mycraft\\shaders\\cube.frag");

    // VERTEX THING
    uint vbo, vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float),
                          (void*) 0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        // TIMING
        float current_frame = (float) glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.2f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

        shader.use();

        fprintf(stderr, "%f %f %f\n",
                camera.position[0],
                camera.position[1],
                camera.position[2]);

        glm::mat4 model = glm::mat4(1.0f);
        shader.set_uniform((char*)"projection", camera.get_projection());
        shader.set_uniform((char*)"view", camera.get_view());
        shader.set_uniform((char*) "model", model);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    if(camera.firstmouse) {
        camera.lastx = (float)x;
        camera.lasty = (float)y;
        camera.firstmouse = false;
    }
    float xoff = (float) x - camera.lastx;
    float yoff = (float) y - camera.lasty;
    camera.lastx = (float) x;
    camera.lasty = (float) y;
    camera.process_cursor(xoff, yoff);
}

internal void
process_input(GLFWwindow* handle)
{
    if(glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(handle, 1);
    if(glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(Direction::FORWARD, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(Direction::BACKWARD, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(Direction::LEFT, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(Direction::RIGHT, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.process_keyboard(Direction::DOWN, delta_time);
    if(glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.process_keyboard(Direction::UP, delta_time);
}

internal void
mouse_callback(GLFWwindow* handle, int button, int action, int mods)
{
    // TODO(fonsi): do something with mouse buttons
}
