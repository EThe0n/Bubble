#include <gl/glew.h>
#pragma comment(lib, "glew32.lib")
#include <gl/glfw3.h>
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#include <time.h>
#include <algorithm>
#include "Program.h"
#include "Grid.h"
#include "Camera.h"
#include "Constant.h"
using namespace Constant;
using namespace std;

int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "GLFW initialization failed \n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x ��Ƽ���ϸ����
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3 �� �� �̴ϴ�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //���� OpenGL�� ������ �ʾƿ�

	GLFWwindow* window = glfwCreateWindow(width, height, "Draw Rect Test", nullptr, nullptr);

	if (window == nullptr) {
		fprintf(stderr, "Failed to open GLFW window \n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "GLEW initialization failed \n");
		exit(EXIT_FAILURE);
	}

	Program* program = nullptr;
	try {
		program = new Program("vertex.glsl", "fragment.glsl");
		program->use();
	}
	catch (const exception& e) {
		printf("%s \n", e.what());
		glfwTerminate();
		exit(EXIT_FAILURE);
	}	

	// �ؿ��� Escape Ű�� �������� ���� ������ �� �ֵ��� �� ��
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	Grid* grid = new Grid(gridXCount, gridYCount, gridCellSize);
	Camera* camera = new Camera(0.5f * gridXCount * gridCellSize, 0.5f * gridYCount * gridCellSize, 2.0f * gridCellSize * max(gridXCount, gridYCount));
	
	GLuint u_mvp = program->getUniformLocation("MVP");
	GLuint u_size = program->getUniformLocation("size");
	glUniform1f(u_size, gridCellSize);

	double lastTime = glfwGetTime();
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		register double currentTime = glfwGetTime();
		register double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		camera->update(window, width, height, deltaTime);
		glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(camera->vp[0][0]));
		grid->render();

		// ���۵��� ��ü
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // ���� ESC Ű�� ���������� Ȥ�� â�� �������� üũ
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	delete program;
	delete grid;

	glfwTerminate();

	return 0;
}