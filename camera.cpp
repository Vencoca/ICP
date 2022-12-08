#include "camera.h"

Camera::Camera()
{
    this->position = glm::vec3(0.0f, 5.0f, 10.0f);
    this->point_of_sight = glm::vec3(0.0f, 5.0f - glm::radians(1.0f), 0.0f);
    this->up_direction = glm::vec3(0, 1, 0);

    glm::vec3 camera_range = this->point_of_sight - this->position;
    double a = camera_range[0];
    double b = camera_range[2];
    this->range = sqrt(a * a + b * b);
    this->Yaw = asin(b / this->range);
}

void Camera::Reset() {
    this->position = glm::vec3(0.0f, 5.0f, 10.0f);
    this->point_of_sight = glm::vec3(0.0f, 5.0f - glm::radians(1.0f), 0.0f);
    this->up_direction = glm::vec3(0, 1, 0);
    glm::vec3 camera_range = this->point_of_sight - this->position;
    double a = camera_range[0];
    double b = camera_range[2];
    this->range = sqrt(a * a + b * b);
    this->Yaw = asin(b / this->range);
}

void Camera::Rotate(bool left) {
    if (left)
    {
        this->Yaw = this->Yaw - glm::radians(10.0f);
    }
    else {
        this->Yaw = this->Yaw + glm::radians(10.0f);
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
    glm::vec3 dir = glm::normalize(camera_range);

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