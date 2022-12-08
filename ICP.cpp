#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <glm/ext.hpp> 
#include "globals.h"
#include "init.h"
#include "callbacks.h"
#include "shaders.h"
#include "verticies.h"
#include "mesh.h"
#include "camera.h"

s_globals globals;
Camera camera = Camera();

int main() {
    init_opengl();
    set_all_callbacks();
    std::vector<GLuint> indices_mesh;
    std::vector<vertex> vertex_mesh;
    GLuint shader_mesh;
    make_shader("resources/basic.vert", "resources/basic.frag", &shader_mesh);

    make_triangle(&indices_mesh, &vertex_mesh);
    mesh_p_c mesh_triangle = mesh_p_c(shader_mesh, vertex_mesh, indices_mesh,GL_TRIANGLES);

    make_checker(&indices_mesh, &vertex_mesh, 32, 32);
    mesh_p_c mesh_checker = mesh_p_c(shader_mesh, vertex_mesh, indices_mesh,GL_TRIANGLE_STRIP);

    int frame_cnt = 0;
    double last_fps = glfwGetTime();

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    float ratio = static_cast<float>(width) / height;
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90� (extra wide) and 30� (quite zoomed in)
        ratio,			     // Aspect Ratio. Depends on the size of your window.
        0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        20000.0f              // Far clipping plane. Keep as little as possible.
    );
    // set visible area
    glViewport(0, 0, width, height);

    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        { 
            // View matrix
            /*
            glm::mat4 v_m = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), //position of camera
                glm::vec3(0.0f, 0.0f, 0.0f), //where to look
                glm::vec3(0, 1, 0)  //UP direction
            );*/

            glm::mat4 v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
            // Model Matrix
            glm::mat4 m_m = glm::identity<glm::mat4>();
            glm::mat4 m_m2 = glm::identity<glm::mat4>();

            m_m = glm::scale(m_m, glm::vec3(5.0f));
            m_m = glm::translate(m_m, glm::vec3(0.0f, 0.25f, 0.0f));
            m_m = glm::rotate(m_m, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));

            mesh_triangle.draw(m_m, v_m, projectionMatrix);
            m_m2 = glm::scale(m_m2, glm::vec3(10.0f));
            m_m2 = glm::rotate(m_m2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            mesh_checker.draw(m_m2, v_m, projectionMatrix);
        }
        glfwSwapBuffers(globals.window);
        glfwPollEvents();
        frame_cnt += 1;
        double now = glfwGetTime();
        if (now - last_fps > 1.0) {
            std::cout << frame_cnt << "FPS\r";
            frame_cnt = 0;
            last_fps = now;
        }
    }
    finalize(1);
    return(EXIT_SUCCESS);
};