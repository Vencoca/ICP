#include "init.h"

extern s_globals globals;

void init_opengl(void)
{
    init_glfw();
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void init_glfw(void)
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // only new functions
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // only old functions (for old tutorials etc.)

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    globals.window = glfwCreateWindow(800, 600, "OpenGL context", NULL, NULL);
    globals.fullscreen = false;
    globals.window_height = 800;
    globals.window_width = 600;
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
    glfwSwapInterval(0);														// Set V-Sync OFF.
    //glfwSwapInterval(1);														// Set V-Sync ON.


    globals.app_start_time = glfwGetTime();										// Get start time.
}

void finalize(int code)
{
    // ...

    // Close OpenGL window if opened and terminate GLFW  
    if (globals.window)
        glfwDestroyWindow(globals.window);
    glfwTerminate();

    // ...
}