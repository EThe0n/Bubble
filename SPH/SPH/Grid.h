#pragma once

#include <GL\glew.h>
#pragma comment(lib, "glew32.lib")

class Grid
{
public:
	Grid(int xCount, int yCount, float cellSize = 1.0f);
	~Grid();

	void render();
private:
	GLfloat*	dev_vertexData;
	
	GLuint		vao;
	GLuint		vertexBuffer;
	GLuint		vertexCount;
};