#include "Grid.h"

#include <exception>

Grid::Grid(int xCount, int yCount, float cellSize)
{
	if (xCount <= 0) {
		throw std::exception("Grid: invalid xCount");
	}
	if (yCount <= 0) {
		throw std::exception("Grid: invalid yCount");
	}
	if (cellSize <= 0.0f) {
		throw std::exception("Grid: invalid cell size");
	}

	vertexCount = 2 * ((xCount + 1) * (yCount + 1));
	register int size = 2 * vertexCount;
	dev_vertexData = new GLfloat[size];

	register int lineCount = 0, iter = 0;
	register float x = 0.0f, y = 0.0f;
	const float xMax = cellSize * xCount, yMax = cellSize * yCount;

	for (register int i = 0; i <= xCount; ++i) {
		iter = lineCount * 4;

		dev_vertexData[iter] = x;
		dev_vertexData[iter + 1] = 0.0f;
		dev_vertexData[iter + 2] = x;
		dev_vertexData[iter + 3] = yMax;

		++lineCount;
		x += cellSize;
	}

	for (register int i = 0; i <= yCount; ++i) {
		iter = lineCount * 4;

		dev_vertexData[iter] = 0.0f;
		dev_vertexData[iter + 1] = y;
		dev_vertexData[iter + 2] = xMax;
		dev_vertexData[iter + 3] = y;

		++lineCount;
		y += cellSize;
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
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_LINES, 0, vertexCount);

	glDisableVertexAttribArray(0);
}
