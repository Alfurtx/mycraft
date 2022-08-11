#include "common.hpp"

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
    FILE* fp = fopen("w:\\mycraft\\shaders\\cube.vert", "rb");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* vertsrc = (char*) _malloca((size + 1));
    fread(vertsrc, 1, size, fp);
    fclose(fp);

    fp = fopen("w:\\mycraft\\shaders\\cube.frag", "rb");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* fragsrc = (char*) _malloca((size + 1));
    fread(fragsrc, 1, size, fp);
    fclose(fp);

    uint vert, frag, prog;
    int  success;
    char infoLog[512];

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertsrc, 0);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragsrc, 0);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(prog, 512, NULL, infoLog);
        fprintf(stderr, "ERROR LINKING SHADER PROGRAM\n%s\n", infoLog);
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    _freea(vertsrc);
    _freea(fragsrc);


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

        glUseProgram(prog);
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
