#include "camera.h"

extern s_globals globals;
extern Camera camera;

Camera::Camera()
{
    this->Reset();
}

void Camera::Reset() {
    this->position = glm::vec3(0.0f, 1.5f, 10.0f);
    //this->point_of_sight = glm::vec3(0.0f, 1.5f - glm::radians(3.0f), 0.0f);
    //this->up_direction = glm::vec3(0, 1, 0);
    this->Yaw = -90.0f;
    this->Roll = 0.0f;
    this->Pitch = 0.0f;
    this->updateCameraVectors();
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
    this->Front = this->point_of_sight - this->position;
    //Lita za kurzorem
    //glm::vec3 dir = glm::normalize(this->Front)*this->MovementSpeed;
    glm::vec3 lrdir = glm::normalize(this->Right) * this->MovementSpeed;
    //Chodi po plose
    glm::vec3 dir = -glm::normalize(glm::cross(this->Right, glm::vec3(0.0f, 1.0f, 0.0f)))*this->MovementSpeed; 

    switch (direction)
    {
    case direction::FORWARD:
        this->position = this->position + dir;
        this->point_of_sight = this->position + this->Front;
        return;
    case direction::BACKWARD:
        this->position = this->position - dir;
        this->point_of_sight = this->position + this->Front;
        return;
    case direction::LEFT:
        this->position = this->position - lrdir;
        this->point_of_sight = this->point_of_sight - lrdir;
        return;
    case direction::RIGHT:
        this->position = this->position + lrdir;
        this->point_of_sight = this->point_of_sight + lrdir;
        return;
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
            camera.Move_with_camera(Camera::direction::LEFT);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::BACKWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::RIGHT);
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


void Camera::updateCameraVectors(){
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(this->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    this->up_direction = glm::normalize(glm::cross(this->Right, this->Front));
    this->point_of_sight = this->position + this->Front;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    if (constraintPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    this->updateCameraVectors();
}