#pragma once
#include <glm/glm.hpp> 

struct vertex {
    glm::vec3 position; // Vertex
    glm::vec3 color; // Color
    glm::vec3 normal;
    glm::vec2 texcoord;
};