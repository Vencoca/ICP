#pragma once

#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <opencv2\opencv.hpp>
#include "vertex.h"
#include "texture.h"

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
    glm::mat4 model_matrix;
    glm::vec3 ambient_material, diffuse_material, specular_material;
    float specular_shinines;
    GLuint texture_id;

    mesh_p_c();
    mesh_p_c(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint& VAO, GLuint primitive);
    mesh_p_c(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint primitive = GL_TRIANGLES);

    void add_material(glm::vec3 ambient_material, glm::vec3 diffuse_material, glm::vec3 specular_material, float specular_shinines) {
        this->ambient_material = ambient_material;
        this->diffuse_material = diffuse_material;
        this->specular_material = specular_material;
        this->specular_shinines = specular_shinines;
    }

    void add_texture_id(const char *path) {
        this->texture_id = textureInit(path, false, false);
    }

    void draw_with_material(const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_position) {
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(model_matrix));
        
        glUniform3fv(glGetUniformLocation(shader_id, "ambient_material"), 1, glm::value_ptr(ambient_material));
        glUniform3fv(glGetUniformLocation(shader_id, "diffuse_material"), 1, glm::value_ptr(diffuse_material));
        glUniform3fv(glGetUniformLocation(shader_id, "specular_material"), 1, glm::value_ptr(specular_material));
        glUniform3fv(glGetUniformLocation(shader_id, "light_position"), 1, glm::value_ptr(light_position));
        glUniform1f(glGetUniformLocation(shader_id, "specular_shinines"), specular_shinines);

        //set texture unit
        glActiveTexture(GL_TEXTURE0);
        //send texture unit number to FS
        glUniform1i(glGetUniformLocation(shader_id, "tex0"), 0);
        // draw object using VAO (Bind+DrawElements+Unbind)
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void draw(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P) {
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(M));
        glBindVertexArray(VAO);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void draw(const glm::mat4& V, const glm::mat4& P) {
        glUseProgram(shader_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
        glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(model_matrix));
        glBindVertexArray(VAO);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void rotate(glm::f32 angle, glm::vec3 axis) {

        model_matrix = glm::rotate(model_matrix, angle, axis);
    };
    void translate(glm::vec3 vector) {

        model_matrix = glm::translate(model_matrix, vector);
    };
    void scale(glm::vec3 vector) {
        model_matrix = glm::scale(model_matrix, vector);
    };

    void reset_mm() {
        model_matrix = glm::identity<glm::mat4>();
    };


    void draw_repeat(const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_position, int repeat, int scale) {
        this->scale(glm::vec3(scale));
        for (int i = 0; i < repeat; i++) {
            for (int j = 0; j < repeat; j++) {
                this->reset_mm();
                this->translate(glm::vec3(float(2*i), 0.0, float(2*j)));
                this->draw_with_material(V, P, light_position);
            }
        }
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
