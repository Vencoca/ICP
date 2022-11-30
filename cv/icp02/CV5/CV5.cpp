// -------------------------------------------- BASIC C++ ------------------------------------------------------
#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
#include <fstream>
#include <sstream>
#include <math.h>
#define _USE_MATH_DEFINES
// ----------------------------------------- INCLUDE GRAPHIC ---------------------------------------------------
#include <opencv2\opencv.hpp> // OpenCV
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> // OpenGL math
#include <glm/ext.hpp> 
// ------------------------------------------- INIT DECLARE ----------------------------------------------------
static void init_opengl(void);
static void init_glfw(void);
static void finalize(int code);
static void init_shaders();
// ----------------------------------------- CALLBACKS DECLARE -------------------------------------------------
void set_all_callbacks();
void use_openGL();
std::string textFileRead(const std::string fn);
std::string getProgramInfoLog(const GLuint obj);
std::string getShaderInfoLog(const GLuint obj);
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void fbsize_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void*
    userParam);
// -------------------------------------------- STRUCTURES -----------------------------------------------------
typedef struct s_globals {
    cv::VideoCapture capture;
    GLFWwindow* window;
    int width;
    int height;
    int window_xpos;
    int window_ypos;
    int window_width;
    int window_height;
    double mouse_xpos;
    double mouse_ypos;
    double app_start_time;
    bool fullscreen;
} s_globals;
s_globals globals;

glm::vec4 color(1,0,0,1);
GLuint shader_program;
// ------------------------------------------------- MAIN -------------------------------------------------------
int main() {
    init_opengl();
    set_all_callbacks();

    //init_shaders();
    GLuint VS_h, FS_h, prog_h;
    VS_h = glCreateShader(GL_VERTEX_SHADER);
    FS_h = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VSsrc = textFileRead("resources/basic.vert");
    const char* VS_string = VSsrc.c_str();
    std::string FSsrc = textFileRead("resources/basic.frag");
    const char* FS_string = FSsrc.c_str();
    glShaderSource(VS_h, 1, &VS_string, NULL);
    glShaderSource(FS_h, 1, &FS_string, NULL);
    glCompileShader(VS_h);
    getShaderInfoLog(VS_h);
    glCompileShader(FS_h);
    getShaderInfoLog(FS_h);
    prog_h = glCreateProgram();
    glAttachShader(prog_h, VS_h);
    glAttachShader(prog_h, FS_h);
    glLinkProgram(prog_h);
    getProgramInfoLog(prog_h);

    GLuint VS_h2, FS_h2, prog2_h;
    VS_h2 = glCreateShader(GL_VERTEX_SHADER);
    FS_h2 = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VSsrc2 = textFileRead("resources/basic2.vert");
    const char* VS_string2 = VSsrc.c_str();
    std::string FSsrc2 = textFileRead("resources/basic2.frag");
    const char* FS_string2 = FSsrc2.c_str();
    glShaderSource(VS_h2, 1, &VS_string2, NULL);
    glShaderSource(FS_h2, 1, &FS_string2, NULL);
    glCompileShader(VS_h2);
    getShaderInfoLog(VS_h2);
    glCompileShader(FS_h2);
    getShaderInfoLog(FS_h2);
    prog2_h = glCreateProgram();
    glAttachShader(prog2_h, VS_h2);
    glAttachShader(prog2_h, FS_h2);
    glLinkProgram(prog2_h);
    getProgramInfoLog(prog2_h);


    //use_openGL();
    //existing data
    struct vertex {
        glm::vec3 position; // Vertex
        glm::vec3 color; // Color
    };
    std::vector<vertex> vertices = {
    { {-0.5f,-0.5f,0.0f}, {1.0f,0.0f,0.0f} },
    { { 0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f} },
    { { 0.0f, 0.5f,0.0f}, {0.0f,0.0f,1.0f} } };
    std::vector<GLuint> indices = { 0,1,2 };
    //GL names for Array and Buffers Objects
    GLuint VAO1;
    {
        GLuint VBO, EBO;
        // Generate the VAO and VBO
        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind VAO (set as the current)
        glBindVertexArray(VAO1);
        // Bind the VBO, set type as GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Fill-in data into the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
        // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Fill-in data into the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        // Set Vertex Attribute to explain OpenGL how to interpret the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
        // Enable the Vertex Attribute 0 = position
        glEnableVertexAttribArray(0);
        // Set end enable Vertex Attribute 1 = Texture Coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
        glEnableVertexAttribArray(1);
        // Bind VBO and VAO to 0 to prevent unintended modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    //------------------------------------------
    // Vygenerujte kolečko uprostřed obrazovky z 100 000 vertexů
    std::vector<vertex> vertices2 = {
    { {0.0f,0.0f,0.0f}, {1.0f,0.0f,0.0f} }, 
    };
    std::vector<GLuint> indices2 = { 0 };
    
    int n = 30;
    float r = 0.45f;
    for (size_t i = 1; i <= n; i++)
    {
        float x = cos(2 * M_PI / n * i) * r;
        float y = sin(2 * M_PI / n * i) * r;
        vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
        vertices2.push_back(vert);
        indices2.push_back(i);
    };
    float x = cos(2 * M_PI / n * 1) * r;
    float y = sin(2 * M_PI / n * 1) * r;
    vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
    vertices2.push_back(vert);
    indices2.push_back(n + 1);
    indices2.push_back(n + 2);
    indices2.push_back(n+3);
    //GL names for Array and Buffers Objects

    GLuint VAO2;
    {
        GLuint VBO, EBO;
        // Generate the VAO and VBO
        glGenVertexArrays(1, &VAO2);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind VAO (set as the current)
        glBindVertexArray(VAO2);
        // Bind the VBO, set type as GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Fill-in data into the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(vertex), vertices2.data(), GL_STATIC_DRAW);
        // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Fill-in data into the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices2.size() * sizeof(GLuint), indices2.data(), GL_STATIC_DRAW);
        // Set Vertex Attribute to explain OpenGL how to interpret the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
        // Enable the Vertex Attribute 0 = position
        glEnableVertexAttribArray(0);
        // Set end enable Vertex Attribute 1 = Texture Coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
        glEnableVertexAttribArray(1);
        // Bind VBO and VAO to 0 to prevent unintended modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    int frame_cnt = 0;
    double last_fps = glfwGetTime();
    while (!glfwWindowShouldClose(globals.window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        //trojúhelník
        {
            glUseProgram(prog_h);
            glBindVertexArray(VAO1);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }

        //kolecko
        {
            glUseProgram(prog2_h); //nemá barvy <- nutno vytvořit
            GLuint loc = glGetUniformLocation(prog2_h, "barva");
            //glUniform4f(loc, 1, 0, 0, 1);
            glUniform4fv(loc, 1, glm::value_ptr(color));
            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLE_FAN, indices2.size(), GL_UNSIGNED_INT, 0);
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
//----------------------------------------------- CALLBACKS -----------------------------------------------------
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
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        std::cout << "W" << "\n";
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        std::cout << "A" << "\n";
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        std::cout << "S" << "\n";
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        std::cout << "D" << "\n";
    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        std::cout << "R" << "\n";
        color = glm::vec4(1, 0, 0, 1);
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS){
        std::cout << "G" << "\n";
        color = glm::vec4(0, 1, 0, 1);
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS){
        std::cout << "B" << "\n";
        color = glm::vec4(0, 0, 1, 1);
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        std::cout << "UP" << "\n";
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        std::cout << "DOWN" << "\n";
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        std::cout << "LEFT" << "\n";
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        std::cout << "RIGHT" << "\n";
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode * mode = glfwGetVideoMode(monitor); 
        if (globals.fullscreen) {
            glfwSetWindowMonitor(window, nullptr, globals.window_xpos, globals.window_ypos, 800, 600, mode->refreshRate);
        } else {
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

//----------------------------------------------- INIT OPENGL ----------------------------------------------------
static void init_opengl(void)
{
    init_glfw();
    glewInit();
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

static void finalize(int code)
{
    // ...

    // Close OpenGL window if opened and terminate GLFW  
    if (globals.window)
        glfwDestroyWindow(globals.window);
    glfwTerminate();

    // ...
}
//--------------------------------------------------  Errory ---------------------------------------------------------------------------
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

//--------------------------------------------------  Shader Support ---------------------------------------------------------------------------
std::string textFileRead(const std::string fn) {
    std::ifstream file;
    std::stringstream ss;
    file.open(fn);
    if (file.is_open()){
        ss << file.rdbuf();
    }
    else {
        std::cerr << "Error opening file: " << fn << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::move(ss.str());
}
std::string getShaderInfoLog(const GLuint obj) {
    int infologLength = 0;
    std::string s;
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::vector<char> v(infologLength);
        glGetShaderInfoLog(obj, infologLength, NULL,
            v.data());
        s.assign(begin(v), end(v));
    }
    return s;
}
std::string getProgramInfoLog(const GLuint obj) {
    int infologLength = 0;
    std::string s;
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::vector<char> v(infologLength);
        glGetProgramInfoLog(obj, infologLength, NULL,
            v.data());
        s.assign(begin(v), end(v));
    }
    return s;
}
//--------------------------------------------------  Shader Init ---------------------------------------------------------------------------
void init_shaders() {
    GLuint VS_h, FS_h, prog_h;
    VS_h = glCreateShader(GL_VERTEX_SHADER);
    FS_h = glCreateShader(GL_FRAGMENT_SHADER);
    std::string VSsrc = textFileRead("resources/basic.vert");
    const char* VS_string = VSsrc.c_str();
    std::string FSsrc = textFileRead("resources/basic.frag");
    const char* FS_string = FSsrc.c_str();
    glShaderSource(VS_h, 1, &VS_string, NULL);
    glShaderSource(FS_h, 1, &FS_string, NULL);
    glCompileShader(VS_h);
    getShaderInfoLog(VS_h);
    glCompileShader(FS_h);
    getShaderInfoLog(FS_h);
    prog_h = glCreateProgram();
    glAttachShader(prog_h, VS_h);
    glAttachShader(prog_h, FS_h);
    glLinkProgram(prog_h);
    getProgramInfoLog(prog_h);
    glUseProgram(prog_h);
}

//--------------------------------------------------  Compute Init ---------------------------------------------------------------------------

void use_openGL() {
    //existing data
    struct vertex {
        glm::vec3 position; // Vertex
        glm::vec3 color; // Color
    };
    std::vector<vertex> vertices = {
    { {-0.5f,-0.5f,0.0f}, {1.0f,0.0f,0.0f} },
    { { 0.5f,-0.5f,0.0f}, {0.0f,1.0f,0.0f} },
    { { 0.0f, 0.5f,0.0f}, {0.0f,0.0f,1.0f} } };
    std::vector<GLuint> indices = { 0,1,2 };
    //GL names for Array and Buffers Objects
    GLuint VAO1; 
    {
        GLuint VBO, EBO;
        // Generate the VAO and VBO
        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // Bind VAO (set as the current)
        glBindVertexArray(VAO1);
        // Bind the VBO, set type as GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Fill-in data into the VBO
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex),vertices.data(), GL_STATIC_DRAW);
        // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // Fill-in data into the EBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),indices.data(), GL_STATIC_DRAW);
        // Set Vertex Attribute to explain OpenGL how to interpret the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),(void*)(0 + offsetof(vertex, position)));
        // Enable the Vertex Attribute 0 = position
        glEnableVertexAttribArray(0);
        // Set end enable Vertex Attribute 1 = Texture Coordinates
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),(void*)(0 + offsetof(vertex, color)));
        glEnableVertexAttribArray(1);
        // Bind VBO and VAO to 0 to prevent unintended modification
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
}