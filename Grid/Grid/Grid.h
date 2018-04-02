#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

#include <glm/glm.hpp>

class Grid
{
public :
	Grid(int xCount, int yCount, float cellSize);
	~Grid();

	void render();

private :
	int numberOfCells;
	GLfloat cellSize;
	
	GLuint instanceBuffer;
	GLuint vertexArrayID;
	GLuint positionBuffer;
	GLuint stateBuffer;
	GLfloat* position;
	GLfloat* state;
	static const GLfloat quadVertex[];

private :
	
};