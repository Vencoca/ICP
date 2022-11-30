#pragma once
#include "vertex.h"
#include <opencv2\opencv.hpp> // OpenCV
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <math.h>

void make_triangle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr);
void make_circle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr);