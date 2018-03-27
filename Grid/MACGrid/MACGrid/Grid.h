#pragma once

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

class Grid
{
public :
	Grid(int width, int height);
	~Grid();

	void render();
	void update();
	
public :
	float* position;
	// 0 : Air
	// 1 : Fluid
	// 2 : Boundary
	GLuint* state;

private :
	static const GLfloat vertexBufferData[8];
	GLuint vertexArrayID;
	GLuint billboardVertexBuffer;
	GLuint positionBuffer;
	GLuint stateBuffer;
	int width;
	int height;
};
