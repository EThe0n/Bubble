#pragma once

#include <GL\glew.h>
#pragma comment(lib, "glew32.lib")

#include "Predefined.hpp"

class Grid
{
public :
	Grid(int xMin, int xMax, int yMin, int yMax, int cellSize = 1);
	~Grid();

	void render();
private :
	GLfloat*	dev_vertexData;
	GLubyte*	dev_colorData;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint vertexCount;
};