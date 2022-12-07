#pragma once
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <opencv2\opencv.hpp>


class Camera
{
public:
    enum class direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up; // camera local UP vector

    GLfloat Yaw = -90.0f;
    GLfloat Pitch = 0.0f;;
    GLfloat Roll = 0.0f;

    // Camera options
    GLfloat MovementSpeed = 1.0f;
    GLfloat MouseSensitivity = 0.25f;
    glm::vec3 WorldUp;

    Camera(glm::vec3 position) :Position(position)
    {
        this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        // initialization of the camera reference system
        this->updateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    void ProcessInput(Camera::direction direction, GLfloat deltaTime)
    {
        GLfloat velocity = MovementSpeed * deltaTime;

        switch (direction)
        {
        case direction::FORWARD:
            this->Front * velocity;
            updateCameraVectors();
            return;
        case direction::BACKWARD:
            -this->Front * velocity;
            updateCameraVectors();
            return;
        case direction::LEFT:
            -this->Right * velocity;
            updateCameraVectors();
            return;
        case direction::RIGHT:
            this->Right* velocity;
            updateCameraVectors();
            return;
        case direction::UP:
            this->Up* velocity;
            updateCameraVectors();
            return;
        case direction::DOWN:
            -this->Up * velocity;
            updateCameraVectors();
            return;

            //...
        }

        return;
    }

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = GL_TRUE)
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

private:
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));

        this->Front = glm::normalize(front);
        this->Right = glm::normalize(glm::cross(this->Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};