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
#include "texture.h"
#include "shaders.h"
#include "arena.h"

s_globals globals;
Camera camera = Camera();

int main() {
    init_opengl();
    set_all_callbacks();
    make_shaders();
    prepare_meshes();

    arena a = arena(15);
    globals.mesh["bunny"].scale(glm::vec3(0.2f));
    globals.mesh["bunny"].translate(glm::vec3(20.0f, 3.0f, 20.0f));
    globals.mesh["teapot"].scale(glm::vec3(0.2f));
    globals.mesh["teapot"].translate(glm::vec3(-20.0f, 3.0f, -20.0f));
    globals.mesh["box"].translate(glm::vec3(-4.0f, 1.0f, 4.0f));

    int frame_cnt = 0;
    double last_fps = glfwGetTime();

    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
        updateInput(globals.window);
        { 
            
            glm::mat4 v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
            a.drawArena(v_m);

            glm::mat4 m_m = glm::identity<glm::mat4>();
            glm::mat4 m_m2 = glm::identity<glm::mat4>();

            m_m = glm::scale(m_m, glm::vec3(5.0f));
            m_m = glm::translate(m_m, glm::vec3(0.0f, 0.25f, 0.0f));
            m_m = glm::rotate(m_m, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));

            globals.mesh["triangle"].draw(m_m, v_m, globals.projectionMatrix);
            
            globals.mesh["bunny"].reset_mm();
            globals.mesh["bunny"].translate(glm::vec3(-20.0f, 10.0f, -20.0f));
            globals.mesh["bunny"].scale(glm::vec3(0.2f));
            //mesh_bunny.rotate(glm::radians(0.1f * (float)glfwGetTime()), glm::vec3(0.1f, 0.2f, 0.4f));
            globals.mesh["bunny"].draw(v_m, globals.projectionMatrix);
           
            globals.mesh["teapot"].reset_mm();
            globals.mesh["teapot"].scale(glm::vec3(0.2f));
            globals.mesh["teapot"].translate(glm::vec3(0.0f, 0.0f, -20.0f));
            
            globals.mesh["teapot"].rotate(glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));
            globals.mesh["teapot"].draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
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
