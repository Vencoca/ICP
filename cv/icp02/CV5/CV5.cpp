// -------------------------------------------- BASIC C++ ------------------------------------------------
#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
// ----------------------------------------- INCLUDE GRAPHIC ---------------------------------------------
#include <opencv2\opencv.hpp> // OpenCV
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> // OpenGL math
// ------------------------------------------- INIT DECLARE -----------------------------------------------
static void init_opengl(void);
static void init_glfw(void);
static void finalize(int code);
// ----------------------------------------- CALLBACKS DECLARE ------------------------------------------
void set_all_callbacks();
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void fbsize_callback(GLFWwindow* window, int width, int height);
// -------------------------------------------- STRUCTURES --------------------------------------------------
typedef struct s_globals {
    cv::VideoCapture capture;
    GLFWwindow* window;
    int width;
    int height;
    double app_start_time;
} s_globals;
s_globals globals;

int main() {
    init_opengl();
    set_all_callbacks();
    while (!glfwWindowShouldClose(globals.window)) {
        glfwPollEvents();
    }
    finalize(1);
    return(EXIT_SUCCESS);
}

//----------------------------------------------- CALLBACKS -----------------------------------------------------
void set_all_callbacks() {
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(globals.window, fbsize_callback);
    glfwSetKeyCallback(globals.window, key_callback);
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void fbsize_callback(GLFWwindow* window, int width, int height)
{
    globals.width = width;
    globals.height = height;

    glMatrixMode(GL_PROJECTION);				// set projection matrix for following transformations
    glLoadIdentity();							// clear all transformations

    glOrtho(0, width, 0, height, -20000, 20000);  // set Orthographic projection

    glViewport(0, 0, width, height);			// set visible area
}


//----------------------------------------------- INIT OPENGL -----------------------------------------------------
static void init_opengl(void)
{
    init_glfw();
}

static void init_glfw(void)
{
    //
    // GLFW init.
    //

        // set error callback first
    glfwSetErrorCallback(error_callback);

    //initialize GLFW library
    int glfw_ret = glfwInit();
    if (!glfw_ret)
    {
        std::cerr << "GLFW init failed." << std::endl;
        finalize(EXIT_FAILURE);
    }

    // Shader based, modern OpenGL (3.3 and higher)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // only new functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // only old functions (for old tutorials etc.)

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    globals.window = glfwCreateWindow(800, 600, "OpenGL context", NULL, NULL);
    if (!globals.window)
    {
        std::cerr << "GLFW window creation error." << std::endl;
        finalize(EXIT_FAILURE);
    }

    // Get some GLFW info.
    {
        int major, minor, revision;

        glfwGetVersion(&major, &minor, &revision);
        std::cout << "Running GLFW " << major << '.' << minor << '.' << revision << std::endl;
        std::cout << "Compiled against GLFW " << GLFW_VERSION_MAJOR << '.' << GLFW_VERSION_MINOR << '.' << GLFW_VERSION_REVISION << std::endl;
    }

    glfwMakeContextCurrent(globals.window);										// Set current window.
    glfwGetFramebufferSize(globals.window, &globals.width, &globals.height);	// Get window size.
    //glfwSwapInterval(0);														// Set V-Sync OFF.
    glfwSwapInterval(1);														// Set V-Sync ON.


    globals.app_start_time = glfwGetTime();										// Get start time.
}

static void finalize(int code)
{
    // ...

    // Close OpenGL window if opened and terminate GLFW  
    if (globals.window)
        glfwDestroyWindow(globals.window);
    glfwTerminate();

    // ...
}