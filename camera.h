#pragma once
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <opencv2\opencv.hpp>
#include "globals.h"

class Camera
{
public:

    glm::vec3 position;
    glm::vec3 point_of_sight;
    glm::vec3 up_direction;

    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

    GLfloat range = 10.0f;

    GLfloat Yaw = 0.0f;
    GLfloat Pitch = 0.0f;;
    GLfloat Roll = 0.0f;
    GLfloat MouseSensitivity = 0.25f;

    GLfloat MovementSpeed = 0.1f;

    enum class direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    Camera();
    void Reset();
    void Rotate(bool left);
    void Move(Camera::direction direction);
    void Move_with_camera(Camera::direction direction);
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = GL_TRUE);
    private :
        void updateCameraVectors();
        
};

void updateInput(GLFWwindow* window);