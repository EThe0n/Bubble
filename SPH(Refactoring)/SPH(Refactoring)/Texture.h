#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment (lib, "opengl32.lib")

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char* path);