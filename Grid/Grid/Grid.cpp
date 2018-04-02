#include "Grid.h"
#include "Constant.h"

using glm::vec2;

const GLfloat Grid::quadVertex[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, 0.5f
};

Grid::Grid(int xCount, int yCount, float cellSize) :
	numberOfCells(xCount * yCount),
	cellSize(cellSize)
{
	const int arraySize = numberOfCells * 2;
	position = new GLfloat[arraySize];

	// set grid position
	const float halfSize = cellSize * 0.5f;
	register float x;
	register float y = halfSize;
	for (register int i = 0; i < yCount; ++i) {
		x = halfSize;
		for (register int j = 0; j < xCount; ++j) {
			register int idx = (i * yCount + j) * 2;
			position[idx] = x;
			position[idx + 1] = y;
			x += cellSize;
		}
		y += cellSize;
	}

	// set grid state
	state = new GLfloat[numberOfCells];
	for (register int i = 0; i < numberOfCells; ++i) {
		state[i] = State::AIR;
	}
	for (register int i = 0; i < xCount; ++i) {
		state[i] = State::BOUNDARY;
	}
	for (register int i = numberOfCells - 1; i >= numberOfCells - xCount; i--) {
		state[i] = State::BOUNDARY;
	}
	for (register int i = yCount; i < numberOfCells; i += xCount) {
		state[i - 1] = State::BOUNDARY;
		state[i] = State::BOUNDARY;
	}
	for (register int i = 0; i < numberOfCells * 0.8; i++) {
		register int x = rand() % (xCount - 2) + 1;
		register int y = rand() % (yCount - 2) + 1;
		state[y * xCount + x] = State::FLUID;
	}

	// set buffers
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertex), quadVertex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(0, 0);

	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), position, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(1, 1);

	glGenBuffers(1, &stateBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, stateBuffer);
	glBufferData(GL_ARRAY_BUFFER, numberOfCells * sizeof(GLfloat), state, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribDivisor(2, 1);
}

Grid::~Grid()
{
	delete[] position;
	delete[] state;

	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &instanceBuffer);
	glDeleteBuffers(1, &stateBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Grid::render()
{
	glBindVertexArray(vertexArrayID);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfCells); 
	glBindVertexArray(0);
}
