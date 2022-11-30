#pragma once
#include <GL/glew.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2\opencv.hpp> // OpenCV

void make_shader(std::string vertex_shader, std::string fragment_shader, GLuint* shader);
std::string textFileRead(const std::string fn);
std::string getProgramInfoLog(const GLuint obj);
std::string getShaderInfoLog(const GLuint obj);