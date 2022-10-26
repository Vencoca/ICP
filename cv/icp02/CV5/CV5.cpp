// C++ 
#include <iostream>
#include <chrono>
#include <numeric>
#include <thread>
// OpenCV 
#include <opencv2\opencv.hpp>

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size);
void draw_cross(cv::Mat& img, int x, int y, int size);
void img_process_code();
cv::Point2f find_center_HSV(cv::Mat& frame);

void init_glew();
static void init_opengl(void);
static void init_glfw(void);
static void finalize(int code);

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void fbsize_callback(GLFWwindow* window, int width, int height);

struct img_data_s {
    cv::Point2f center_relative;
    cv::Mat frame;
};

typedef struct s_globals {
    cv::VideoCapture capture;
    GLFWwindow* window;
    int width;
    int height;
    double app_start_time;
} s_globals;

s_globals globals;
std::unique_ptr<img_data_s> img_data_p;
std::mutex my_mutex;

int main() {
    cv::Mat frame;
    globals.capture = cv::VideoCapture(cv::CAP_DSHOW);
    std::unique_ptr<img_data_s> img_data_local;
    //cv::namedWindow("frame");
    bool new_data = false;
    std::thread img_thread(img_process_code);
    while (globals.capture.isOpened()) {
        my_mutex.lock();
        if (img_data_p && !img_data_p->frame.empty()) {
            img_data_local = std::move(img_data_p);
            new_data = true;
        }
        else {
            new_data = false;
        }
        my_mutex.unlock();
        if (new_data) {
            std::cout << "sted zarovky relativne" << img_data_local->center_relative << "\n";
            //draw_cross_relative(img_data_local->frame, img_data_local->center_relative, 25);
            //cv::imshow("frame", img_data_local->frame);
        }
        else {
            std::cout << ".";
        }
        cv::waitKey(16);
    }
    if (img_thread.joinable()) {
        img_thread.join();
    }

    return(EXIT_SUCCESS);
}

void img_process_code() {
    cv::Mat frame;
    while (globals.capture.isOpened()) {
        globals.capture.read(frame);
        if (frame.empty()) {
            std::cerr << "device closed" << "\n";
            globals.capture.release();
            break;
        }
        //create new result
        std::unique_ptr<img_data_s> img_data_local = std::make_unique<img_data_s>();
        //compute new result
        img_data_local->center_relative = find_center_HSV(frame);
        frame.copyTo(img_data_local->frame);

        //synced send
        my_mutex.lock();
        img_data_p = std::move(img_data_local);
        my_mutex.unlock();
        //simulate 10 FPS
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void draw_cross(cv::Mat& img, int x, int y, int size)
{
    cv::Point p1(x - size / 2, y);
    cv::Point p2(x + size / 2, y);
    cv::Point p3(x, y - size / 2);
    cv::Point p4(x, y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

void draw_cross_relative(cv::Mat& img, cv::Point2f center_relative, int size)
{
    center_relative.x = std::clamp(center_relative.x, 0.0f, 1.0f);
    center_relative.y = std::clamp(center_relative.y, 0.0f, 1.0f);
    size = std::clamp(size, 1, std::min(img.cols, img.rows));

    cv::Point2f center_absolute(center_relative.x * img.cols, center_relative.y * img.rows);

    cv::Point2f p1(center_absolute.x - size / 2, center_absolute.y);
    cv::Point2f p2(center_absolute.x + size / 2, center_absolute.y);
    cv::Point2f p3(center_absolute.x, center_absolute.y - size / 2);
    cv::Point2f p4(center_absolute.x, center_absolute.y + size / 2);

    cv::line(img, p1, p2, CV_RGB(255, 0, 0), 3);
    cv::line(img, p3, p4, CV_RGB(255, 0, 0), 3);
}

cv::Point2f find_center_HSV(cv::Mat& frame) {
    cv::Mat frame_hsv;
    cv::cvtColor(frame, frame_hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower_bound(55, 50, 50);
    cv::Scalar upper_bound(65, 255, 255);
    cv::Mat frame_treshold;
    cv::inRange(frame_hsv, lower_bound, upper_bound, frame_treshold);

    //cv::namedWindow("frametr");
    //cv::imshow("frametr", frame_treshold);

    std::vector<cv::Point> white_pixels;
    cv::findNonZero(frame_treshold, white_pixels);
    cv::Point white_reduced = std::reduce(white_pixels.begin(), white_pixels.end());

    cv::Point2f center_relative((float)white_reduced.x / white_pixels.size() / frame.cols, (float)white_reduced.y / white_pixels.size() / frame.rows);
    return center_relative;
}

void init_glew(void)
{
    //
    // Initialize all valid GL extensions with GLEW.
    // Usable AFTER creating GL context!
    //
    {
        GLenum glew_ret;
        glew_ret = glewInit();
        if (glew_ret != GLEW_OK)
        {
            std::cerr << "WGLEW failed with error: " << glewGetErrorString(glew_ret) << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "GLEW successfully initialized to version: " << glewGetString(GLEW_VERSION) << std::endl;
        }
        // Platform specific. (Change to GLXEW or ELGEW if necessary.)
        glew_ret = wglewInit();
        if (glew_ret != GLEW_OK)
        {
            std::cerr << "WGLEW failed with error: " << glewGetErrorString(glew_ret) << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "WGLEW successfully initialized platform specific functions." << std::endl;
        }
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

static void init_opengl(void)
{
    init_glfw();
}

// in file init.cpp
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