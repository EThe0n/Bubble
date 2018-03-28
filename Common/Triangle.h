#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

class Triangle
{
public :
	Triangle();
	~Triangle();

	void render();

private :
	GLuint vertexBuffer;
	GLuint vertexArrayID;
	static const GLfloat vertexBufferData[];
};