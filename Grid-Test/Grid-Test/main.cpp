#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

#include "Shader.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Texture.hpp"
#include "Simulator.hpp"
#include "Predefined.hpp"

int main()
{
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Particle-Test", nullptr, nullptr);
	if (!window) {
		cerr << "Failed to Open GLFW Window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the Tutorial" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to Initialize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwPollEvents();
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	glUseProgram(programID);

	// uniform variable - vertex
	GLuint u_mvp = glGetUniformLocation(programID, "MVP");
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint channel = glGetUniformLocation(programID, "channel");

	// texture - fragment
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint texture = loadDDS("particle.DDS");

	Simulator particles = Simulator();
	Camera camera = Camera(vec3((X_MAX + X_MIN) / 2.0f, (Y_MAX + Y_MIN) / 2.0f, X_MAX * 1.5));
	Grid grid = Grid(X_MIN, X_MAX, Y_MIN, Y_MAX, CELL_SIZE); 

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

		// set camera
		camera.computeMatricesFromInputs(window);
		mat4 p = camera.getProjection();
		mat4 v = camera.getView();
		vec3 cameraPosition(glm::inverse(v)[3]);
		mat4 mvp = camera.getProjection() * camera.getView();

		particles.simulate(delta);
		particles.update();

		// render
		
		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mvp[0][0]);
		glUniform3f(CameraRight_worldspace_ID, v[0][0], v[1][0], v[2][0]);
		glUniform3f(CameraUp_worldspace_ID, v[0][1], v[1][1], v[2][1]);

		glUniform1i(channel, PARTICLE_CHANNEL);
		particles.render();

		glUniform1i(channel, GRID_CHANNEL);
		grid.render();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	glDeleteProgram(programID);
	glfwTerminate();

	return 0;
}