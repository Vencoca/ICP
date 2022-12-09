#include "callbacks.h"

extern s_globals globals;
extern Camera camera;

void set_all_callbacks() {
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(globals.window, fbsize_callback);
    glfwSetKeyCallback(globals.window, key_callback);
    glfwSetCursorPosCallback(globals.window, cursor_position_callback);
    glfwSetMouseButtonCallback(globals.window, mouse_button_callback);

    if (glfwExtensionSupported("GL_ARB_debug_output"))
    {
        glDebugMessageCallback(MessageCallback, 0);
        //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        std::cout << "GL_DEBUG enabled." << std::endl;
    }
}

void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camera.Reset();
        std::cout << "R" << "\n";
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (globals.fullscreen) {
            glfwSetWindowMonitor(window, nullptr, globals.window_xpos, globals.window_ypos, 800, 600, mode->refreshRate);
        }
        else {
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        globals.fullscreen = !globals.fullscreen;
    }
}

void fbsize_callback(GLFWwindow* window, int width, int height)
{
    globals.width = width;
    globals.height = height;
    std::cout << "Height: " << height << " Width: " << width << "\n";

    glMatrixMode(GL_PROJECTION);				// set projection matrix for following transformations
    glLoadIdentity();							// clear all transformations

    glOrtho(0, width, 0, height, -20000, 20000);  // set Orthographic projection

    glViewport(0, 0, width, height);			// set visible area
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    globals.mouse_xpos = xpos;
    globals.mouse_ypos = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        std::cout << "xpos: " << globals.mouse_xpos << " ypos: " << globals.mouse_ypos << "\n";
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void*
    userParam)
{
    auto const src_str = [source]() {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "Unknown";
        }
    }();
    auto const type_str = [type]() {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "Unknown";
        }
    }();
    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        default: return "Unknown";
        }
    }();
    std::cout << "[GL CALLBACK]: " <<
        "source = " << src_str <<
        ", type = " << type_str <<
        ", severity = " << severity_str <<
        ", ID = '" << id << '\'' <<
        ", message = '" << message << '\'' << std::endl;
}