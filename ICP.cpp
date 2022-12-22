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
#include "OBJloader.h"

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


   
    
    mesh_p_c mesh_bunny = mesh_p_c();
    createMesh("resources/bunny_tri_vnt.obj", shader_mesh, mesh_bunny, { 1.0f,0.0f,0.0f });
    mesh_bunny.scale(glm::vec3(0.2f));
    mesh_bunny.translate(glm::vec3(20.0f, 3.0f, 20.0f));

    mesh_p_c mesh_teapot = mesh_p_c();
    createMesh("resources/teapot_tri_vnt.obj", shader_mesh, mesh_teapot, { 0.0f,1.0f,0.0f });
    mesh_teapot.scale(glm::vec3(0.2f));
    mesh_teapot.translate(glm::vec3(-20.0f, 3.0f, -20.0f));


    int frame_cnt = 0;
    double last_fps = glfwGetTime();

    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        updateInput(globals.window);
        { 

            glm::mat4 v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
            glm::mat4 m_m = glm::identity<glm::mat4>();
            glm::mat4 m_m2 = glm::identity<glm::mat4>();

            m_m = glm::scale(m_m, glm::vec3(5.0f));
            m_m = glm::translate(m_m, glm::vec3(0.0f, 0.25f, 0.0f));
            m_m = glm::rotate(m_m, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));

            mesh_triangle.draw(m_m, v_m, globals.projectionMatrix);
            m_m2 = glm::scale(m_m2, glm::vec3(10.0f));
            m_m2 = glm::rotate(m_m2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            mesh_checker.draw(m_m2, v_m, globals.projectionMatrix);
            
            mesh_bunny.translate(glm::vec3(0.0f, 0.005f, 0.0f));
            mesh_bunny.rotate(glm::radians(0.1f * (float)glfwGetTime()), glm::vec3(0.1f, 0.2f, 0.4f));
            mesh_bunny.draw(v_m, globals.projectionMatrix);

            mesh_teapot.draw(v_m, globals.projectionMatrix);
        }
        glfwSwapBuffers(globals.window);
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