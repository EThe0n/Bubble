#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
