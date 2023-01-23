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

s_globals globals;
Camera camera = Camera();
GLuint gen_tex(std::string filepath);

int main() {
    init_opengl();
    set_all_callbacks();
    make_shaders();

    std::vector<GLuint> indices_mesh;
    std::vector<vertex> vertex_mesh;
    make_triangle(&indices_mesh, &vertex_mesh);
    mesh mesh_triangle = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);

    mesh mesh_floor = mesh();
    createMesh("resources/floor.obj", globals.shader["texture"], mesh_floor, { 1.0f,1.0f,1.0f });
    mesh_floor.add_texture_id("resources/floor.jpg");
    mesh_floor.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 0.0);

    mesh mesh_bunny = mesh();
    createMesh("resources/bunny_tri_vnt.obj", globals.shader["mesh"], mesh_bunny, { 1.0f,0.0f,0.0f });
    mesh_bunny.scale(glm::vec3(0.2f));
    mesh_bunny.translate(glm::vec3(20.0f, 3.0f, 20.0f));

    mesh mesh_teapot = mesh();
    createMesh("resources/teapot_tri_vnt.obj", globals.shader["light"], mesh_teapot, { 0.0f,1.0f,0.0f });
    mesh_teapot.scale(glm::vec3(0.2f));
    mesh_teapot.translate(glm::vec3(-20.0f, 3.0f, -20.0f));
    mesh_teapot.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);

    mesh mesh_box = mesh();
    createMesh("resources/box.obj", globals.shader["texture"], mesh_box, { 0.0f, 0.0f, 0.0f });
    mesh_box.translate(glm::vec3(-4.0f, 1.0f, 4.0f));
    mesh_box.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);
    mesh_box.add_texture_id("resources/box.png");

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


            mesh_floor.draw_repeat(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f), 15,1 );
            
            mesh_bunny.reset_mm();
            mesh_bunny.translate(glm::vec3(-20.0f, 10.0f, -20.0f));
            mesh_bunny.scale(glm::vec3(0.2f));
            //mesh_bunny.rotate(glm::radians(0.1f * (float)glfwGetTime()), glm::vec3(0.1f, 0.2f, 0.4f));
            mesh_bunny.draw(v_m, globals.projectionMatrix);
           
            mesh_teapot.reset_mm();
            mesh_teapot.scale(glm::vec3(0.2f));
            mesh_teapot.translate(glm::vec3(0.0f, 0.0f, -20.0f));
            
            mesh_teapot.rotate(glm::radians(100.0f * (float)glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));
            mesh_teapot.draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));

            mesh_box.reset_mm();
            mesh_box.translate(glm::vec3(0.0f, 1.001f, 0.0f));
            mesh_box.draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
            mesh_box.translate(glm::vec3(30.0f, 0.0f, 30.0f));
            mesh_box.draw_with_material(v_m, globals.projectionMatrix, glm::vec3(-20.0f, 10.0f, -20.0f));
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


GLuint gen_tex(std::string filepath)
{
    GLuint ID;
    cv::Mat image = cv::imread(filepath, cv::IMREAD_UNCHANGED); // Read with (potential) Alpha
    //if (image.channels() != 4) exit(-1);  // Check the image, we want Alpha in this example    

    //cv::imshow("Display window", image);
    //int k = cv::waitKey(0); // Wait for a keystroke in the window

    // Generates an OpenGL texture object
    glGenTextures(1, &ID);

    // Assigns the texture to a Texture Unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Texture data alignment for transfer (single byte = basic, slow, but safe option; usually not necessary) 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Assigns the image to the OpenGL Texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);

    // Configures the type of algorithm that is used to make the image smaller or bigger
    // nearest neighbor - ugly & fast 
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // bilinear - nicer & slower
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // MIPMAP filtering + automatic MIPMAP generation - nicest, needs more memory. Notice: MIPMAP is only for image minifying.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilinear magnifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear minifying
    glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now.

    // Configures the way the texture repeats
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Unbinds the OpenGL Texture object so that it can't accidentally be modified
    glBindTexture(GL_TEXTURE_2D, 0);
    return ID;
}