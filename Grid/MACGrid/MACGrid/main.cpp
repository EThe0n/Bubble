#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "Camera.hpp"
#include "Shader.hpp"
#include "Grid.h"
#include "Predefined.h"

int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW \n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Particle-Test", nullptr, nullptr);
	if (!window) {
		fprintf(stderr, "Failed to Open GLFW Window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the Tutorial \n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW \n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, Predefined::Width / 2.0f, Predefined::Height / 2.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	glUseProgram(programID);

	// uniform variable - vertex
	GLuint u_mvp = glGetUniformLocation(programID, "MVP");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera camera = Camera(vec3((Predefined::cellCountX) * 0.5f, (Predefined::cellCountY) * 0.5f, (Predefined::cellCountX * 1.5f)));
	Grid* grid = new Grid(Predefined::cellCountX, Predefined::cellCountY);

	double lastTime = glfwGetTime();
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		camera.computeMatricesFromInputs(window, Predefined::Width, Predefined::Height);
		mat4 mvp = camera.getProjection() * camera.getView();

		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mvp[0][0]);

		grid->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	delete grid;

	glfwTerminate();

	return 0;
}