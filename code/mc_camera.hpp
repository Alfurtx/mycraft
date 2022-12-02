#if !defined(MYCRAFT_CAMERA_H)
#define MYCRAFT_CAMERA_H

#include "common.hpp"

#define CAMERA_RENDER_DISTANCE 20.0f
#define CAMERA_YAW -90.0f
#define CAMERA_PITCH 0.0f
#define CAMERA_SPEED 2.5f
#define CAMERA_SENSITIVITY 0.1f
#define CAMERA_FOV 45.0f

struct Camera {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float lastx;
    float lasty;

    bool firstmouse;
    bool speedup;

    void init();
    glm::mat4 get_view();
    glm::mat4 get_projection();
    void process_keyboard(Direction direction, float delta_time);
    void process_cursor(float x_offset, float y_offset);
};

#endif
