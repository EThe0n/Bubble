#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <cstdio>
#include <cstdlib>
#include <exception>

#include "Simulator.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.h"
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

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Particle-Test", nullptr, nullptr);
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
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	glUseProgram(programID);

	// uniform variable - vertex
	GLuint u_mvp = glGetUniformLocation(programID, "MVP");
	GLuint channel = glGetUniformLocation(programID, "channel");
	GLuint particleSize = glGetUniformLocation(programID, "particleSize");

	// texture - fragment
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint texture = loadDDS("particle.DDS");
	Camera camera = Camera(vec3((GRID_X_COUNT * CELL_SIZE) * 0.5f, (GRID_X_COUNT * CELL_SIZE) * 0.5f, (GRID_X_COUNT * CELL_SIZE) * 1.5));
	Simulator* simulator = nullptr;

	try {
		simulator = new Simulator();
	}
	catch (std::exception e) {
		fprintf(stderr, "%s \n", e.what());
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureID, 0);

	double lastTime = glfwGetTime();
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// get time
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;

		camera.computeMatricesFromInputs(window);
		mat4 mvp = camera.getProjection() * camera.getView();

		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mvp[0][0]);

		try {
			simulator->update();
			simulator->simulate(delta * 0.2f);
			simulator->render(channel, particleSize);
		}
		catch (std::exception& e) {
			fprintf(stderr, "%s \n", e.what());
			delete simulator;

			glDeleteProgram(programID);
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	delete simulator;

	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}