#include "common.hpp"
#include "mycraft_renderer.hpp"

internal void resize_window_callback(GLFWwindow* handle, int w, int h);
internal void cursor_position_callback(GLFWwindow* handle, double x, double y);
internal void keyboard_callback(GLFWwindow* handle, int key, int scancode, int action, int mods);
internal void mouse_callback(GLFWwindow* handle, int button, int action, int mods);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

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
    glfwSetKeyCallback(window, keyboard_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            fprintf(stderr, "Failed to inititialize GLAD\n");
            exit(-1);
    }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) ,(void*) 0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use_program();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    // TODO(fonsi): camera processing
}

internal void
keyboard_callback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    if(GLFW_PRESS == action) {
        if(GLFW_KEY_ESCAPE == key) {
            glfwSetWindowShouldClose(handle, 1);
        }
    }
}

internal void
mouse_callback(GLFWwindow* handle, int button, int action, int mods)
{
    // TODO(fonsi): do something with mouse buttons
}
