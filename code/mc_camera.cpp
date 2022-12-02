#include "mc_camera.hpp"

INTERNAL void
update_vectors(Camera& camera)
{
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    camera.right = glm::normalize(glm::cross(camera.front, camera.world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}

void
Camera::init()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    yaw = CAMERA_YAW;
    firstmouse = true;
    pitch = CAMERA_PITCH;
    speed = CAMERA_SPEED;
    sensitivity = CAMERA_SENSITIVITY;
    world_up = up;
    lastx = SCREEN_WIDTH / 2.0f;
    lasty = SCREEN_HEIGHT / 2.0f;
    update_vectors(*this);
    speedup = false;
}

glm::mat4
Camera::get_view()
{
    return(glm::lookAt(position, position + front, up));
}

glm::mat4
Camera::get_projection()
{
    return(glm::perspective(glm::radians(45.0f),
                           SCREEN_WIDTH / SCREEN_HEIGHT,
                           0.1f,
                           1000.f));
}

void
Camera::process_keyboard(Direction direction, float delta_time)
{
    float32 s = speedup ? speed * 10 : speed;
    float velocity = s * delta_time;
    if (direction == Direction::FORWARD)
        position += front * velocity;
    if (direction == Direction::BACKWARD)
        position -= front * velocity;
    if (direction == Direction::LEFT)
        position -= right * velocity;
    if (direction == Direction::RIGHT)
        position += right * velocity;
    if (direction == Direction::DOWN)
        position -= up * velocity;
    if (direction == Direction::UP)
        position += up * velocity;
}

void
Camera::process_cursor(float x_offset, float y_offset)
{
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw   += x_offset;
    pitch -= y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if(firstmouse) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    update_vectors(*this);
}
