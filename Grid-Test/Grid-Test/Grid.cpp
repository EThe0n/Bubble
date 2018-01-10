#include <cmath>

#include "Grid.hpp"

Grid::Grid(int xMin, int xMax, int yMin, int yMax, int cellSize)
{
	vertexCount = 2 * ((xMax - xMin) / cellSize + (yMax - yMin) / cellSize + 2);
	const int size = 3 * vertexCount;
	dev_vertexData = new GLfloat[size];

	register int lineCount = 0;
	for (register int x = xMin; x <= xMax; x += cellSize, ++lineCount) {
		register int iter = lineCount * 6;

		dev_vertexData[iter]		= x;
		dev_vertexData[iter + 1]	= yMin;
		dev_vertexData[iter + 2]	= 0.0f;

		dev_vertexData[iter + 3]	= x;
		dev_vertexData[iter + 4]	= yMax;
		dev_vertexData[iter + 5]	= 0.0f;
	}
	
	for (register int y = yMin; y <= yMax; y += cellSize, ++lineCount) {
		register int iter = lineCount * 6;

		dev_vertexData[iter]		= xMin;
		dev_vertexData[iter + 1]	= y;
		dev_vertexData[iter + 2]	= 0.0f;

		dev_vertexData[iter + 3]	= xMax;
		dev_vertexData[iter + 4]	= y;
		dev_vertexData[iter + 5]	= 0.0f;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), dev_vertexData, GL_STATIC_DRAW);
}

Grid::~Grid()
{
	delete[] dev_vertexData;

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vao);
}

void Grid::render()
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_LINES, 0, vertexCount);

	glDisableVertexAttribArray(0);
}
