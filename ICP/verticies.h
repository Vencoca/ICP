#pragma once
#include "vertex.h"
#include <opencv2\opencv.hpp> // OpenCV
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <math.h>

void make_triangle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr);
void make_triangle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr);
void make_circle(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr);
void make_circle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr);
void make_checker(GLuint* VAO_ptr, std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr,  size_t number_of_col, size_t number_of_rows);
void make_checker(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr, size_t number_of_col, size_t number_of_rows);