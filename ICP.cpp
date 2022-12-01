#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <glm/ext.hpp> 
#include "globals.h"
#include "init.h"
#include "callbacks.h"
#include "shaders.h"
#include "verticies.h"
s_globals globals;
int main() {
    init_opengl();
    set_all_callbacks();
    //--------------------------- First Object -----------------------------
    GLuint shader_one;
    make_shader("resources/basic.vert", "resources/basic.frag", &shader_one);
    GLuint VAO_one;
    std::vector<GLuint> indices_one;
    make_triangle(&VAO_one, &indices_one);
    //--------------------------- Second Object -----------------------------
    GLuint shader_two;
    make_shader("resources/basic2.vert", "resources/basic2.frag", &shader_two);
    GLuint VAO_two;
    std::vector<GLuint> indices_two;
    make_circle(&VAO_two, &indices_two);
    //-----------------------------------------------------------------------
    int frame_cnt = 0;
    double last_fps = glfwGetTime();



    glUseProgram(shader_one);
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

    float ratio = static_cast<float>(width) / height;

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90� (extra wide) and 30� (quite zoomed in)
        ratio,			     // Aspect Ratio. Depends on the size of your window.
        0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        20000.0f              // Far clipping plane. Keep as little as possible.
    );

    //set uniform for shaders - projection matrix
    glUniformMatrix4fv(glGetUniformLocation(shader_one, "uProj_m"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // set visible area
    glViewport(0, 0, width, height);


    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*
        { // Circle
            glUseProgram(shader_two);
            GLuint loc = glGetUniformLocation(shader_two, "barva");
            glUniform4fv(loc, 1, glm::value_ptr(globals.color));
            glBindVertexArray(VAO_two);
            glDrawElements(GL_TRIANGLE_FAN, indices_two.size(), GL_UNSIGNED_INT, 0);
        }
        */
        { //Triangle
            glUseProgram(shader_one);

            // View matrix
            glm::mat4 v_m = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), //position of camera
                glm::vec3(0.0f, 0.0f, 0.0f), //where to look
                glm::vec3(0, 1, 0)  //UP direction
            );
            glUniformMatrix4fv(glGetUniformLocation(shader_one, "uV_m"), 1, GL_FALSE, glm::value_ptr(v_m));


            // Model Matrix
            glm::mat4 m_m = glm::identity<glm::mat4>();

            //m_m = glm::translate(m_m, glm::vec3(width / 2.0, height / 2.0, 0.0));
            m_m = glm::scale(m_m, glm::vec3(5.0f));
            m_m = glm::rotate(m_m, glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.2f, 0.1f, 0.3f));

            glUniformMatrix4fv(glGetUniformLocation(shader_one, "uMV_m"), 1, GL_FALSE, glm::value_ptr(m_m));
            // =====================================================================================================

            // modify Model matrix and send to shaders
            // rotate slowly



            glBindVertexArray(VAO_one);
            glDrawElements(GL_TRIANGLES, indices_one.size(), GL_UNSIGNED_INT, 0);
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