#include "camera.h"

extern s_globals globals;
extern Camera camera;

Camera::Camera()
{
    this->Reset();
}

void Camera::Reset() {
    this->position = glm::vec3(0.0f, 1.5f, 10.0f);
    this->point_of_sight = glm::vec3(0.0f, 1.5f - glm::radians(3.0f), 0.0f);
    this->up_direction = glm::vec3(0, 1, 0);
    glm::vec3 camera_range = this->point_of_sight - this->position;
    double a = camera_range[0];
    double b = camera_range[2];
    this->range = sqrt(a * a + b * b);
    this->Yaw = asin(b / this->range);
    this->Roll = 0.0f;
    this->Pitch = 0.0f;
}

void Camera::Rotate(bool left) {
    if (left)
    {
        this->Yaw = this->Yaw - glm::radians(2.0f);
    }
    else {
        this->Yaw = this->Yaw + glm::radians(2.0f);
    }
    double new_x = cos(this->Yaw) * this->range;
    double new_y = sin(this->Yaw) * this->range;
    this->point_of_sight = glm::vec3({ this->position[0] + new_x, this->point_of_sight[1], this->position[2] + new_y });
}

void Camera::Move(Camera::direction direction) {
    switch (direction)
    {
    case direction::FORWARD:
        this->position = this->position + glm::vec3(0.0, 0.0, -1.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(0.0, 0.0, -1.0);
        return;
    case direction::BACKWARD:
        this->position = this->position + glm::vec3(0.0, 0.0, 1.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(0.0, 0.0, 1.0);
        return;
    case direction::LEFT:
        this->position = this->position + glm::vec3(-1.0, 0.0, 0.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(-1.0, 0.0, 0.0);
        return;
    case direction::RIGHT:
        this->position = this->position + glm::vec3(1.0, 0.0, 0.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(1.0, 0.0, 0.0);
        return;
    }
}

void Camera::Move_with_camera(Camera::direction direction) {
    glm::vec3 camera_range = this->point_of_sight - this->position;
    glm::vec3 dir = glm::normalize(camera_range)*this->MovementSpeed;

    switch (direction)
    {
    case direction::FORWARD:
        this->position = this->position + dir;
        this->point_of_sight = this->position + camera_range;
        return;
    case direction::BACKWARD:
        this->position = this->position - dir;
        this->point_of_sight = this->position + camera_range;
        return;
    case direction::LEFT:
        break;
    case direction::RIGHT:
        break;
    }
}

void updateInput(GLFWwindow* window) {
    double now = glfwGetTime();
    if (now - globals.last_update > 0.01) {
        globals.last_update = now;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::FORWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.Rotate(true);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::BACKWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.Rotate(false);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera.Move(Camera::direction::FORWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.Move(Camera::direction::BACKWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera.Move(Camera::direction::LEFT);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera.Move(Camera::direction::RIGHT);
        }
    }
}
