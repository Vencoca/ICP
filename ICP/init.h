#pragma once
#include <iostream>
#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include "globals.h"
#include "callbacks.h"

void init_opengl(void);
void init_glfw(void);
void finalize(int code);