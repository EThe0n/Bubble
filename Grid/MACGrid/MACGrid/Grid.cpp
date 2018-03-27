#include "Grid.h"

#include <memory>

const GLfloat Grid::vertexBufferData[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	-0.5f, 0.5f,
	0.5f, 0.5f
};

Grid::Grid(int width, int height)
{
	const int cellCount = width * height;
	const int arrSize = cellCount * 2;

	position	= new float[arrSize];
	state		= new GLuint[cellCount];

	// initialize position and state
	register int x, y = 0.5f; 
	register int idx;
	for (register int i = 0; i < width; ++i) {
		x = 0.5f;
		for (register int j = 0; j < height; j++) {
			idx = (i * width + j) * 2;
			position[idx] = x;
			position[idx + 1] = y;
			x += 1.0f;
		}
		y += 1.0f;
	}
	memset(state, 0, sizeof(GLuint) * cellCount);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);

	glGenBuffers(1, &stateBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, stateBuffer);

	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, arrSize * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);
}

Grid::~Grid()
{
	delete[] position;
	delete[] state;

	glDeleteBuffers(1, &positionBuffer);
	glDeleteBuffers(1, &billboardVertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Grid::render()
{
	const int n = width * height;

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : (x, y) = 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3nd attribute buffer : state buffer
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, stateBuffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		1,                                // size : (x, y) = 2
		GL_UNSIGNED_INT,                  // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1

								 // Draw the particules !
								 // This draws many times a small triangle_strip (which looks like a quad).
								 // This is equivalent to :
								 // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
								 // but faster!
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Grid::update()
{
	const int n = width * height;
	const int arraySize = n * 2;

	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, arraySize * sizeof(GLfloat), position);

	// state buffer 
}
