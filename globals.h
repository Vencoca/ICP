#pragma once

#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <vector>
#include <map>
#include "mesh.h"


struct s_globals {
    GLFWwindow* window;
    int width;
    int height;
    int window_xpos;
    int window_ypos;
    int window_width;
    int window_height;
    double mouse_xpos;
    double mouse_ypos;
    double app_start_time;
    bool fullscreen;
    double last_update;
    float fov;
    glm::vec4 color;
    glm::mat4 projectionMatrix;
    std::map<std::string, GLuint> shader;
    //std::map<std::string, mesh> meshh;
};