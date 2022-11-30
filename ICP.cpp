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

    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        { // Circle
            glUseProgram(shader_two);
            GLuint loc = glGetUniformLocation(shader_two, "barva");
            glUniform4fv(loc, 1, glm::value_ptr(globals.color));
            glBindVertexArray(VAO_two);
            glDrawElements(GL_TRIANGLE_FAN, indices_two.size(), GL_UNSIGNED_INT, 0);
        }
        { //Triangle
            glUseProgram(shader_one);
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
}