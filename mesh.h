#pragma once

#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <opencv2\opencv.hpp>
#include "vertex.h"

struct vertex_p {
    glm::vec3 position;
};

// mesh classes
class Mesh {};


class mesh_p_c {
public:
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO;
    GLenum primitive;
    GLuint shader_id;

    mesh_p_c(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint& VAO, GLuint primitive);
    mesh_p_c(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint primitive = GL_TRIANGLES);

    void draw(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P) {
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(M));

        glBindVertexArray(VAO);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }
};

class mesh_p_m {
public:
    std::vector<vertex_p> vertices;
    std::vector<GLuint> indices;
    GLuint shader_id;
    GLuint VAO;
    GLenum primitive;
    glm::vec4 material;

    mesh_p_m(GLuint shader_id, std::vector<vertex_p>& vertices, std::vector<GLuint>& indices);

    void draw(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P) {
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(M));
        glUniform4fv(glGetUniformLocation(shader_id, "material"), 1, glm::value_ptr(material));

        glBindVertexArray(VAO);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }
};
