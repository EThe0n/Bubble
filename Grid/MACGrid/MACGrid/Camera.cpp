#define _USE_MATH_DEFINES
#include <math.h>
#include "Camera.hpp"

using glm::cross;
using glm::perspective;
using glm::lookAt;
using glm::radians;

void Camera::computeMatricesFromInputs(GLFWwindow * window, int width, int height)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	const float centerOfWidth	= width / 2;
	const float centerOfHeight	= height / 2;

	glfwSetCursorPos(window, centerOfWidth, centerOfHeight);

	vec3 direction(
		cosf(verticalAngle) * sinf(horizontalAngle),
		sinf(verticalAngle),
		cosf(verticalAngle) * cosf(horizontalAngle)
	);
	vec3 right(
		sinf(horizontalAngle - M_PI),
		0.0f,
		cosf(horizontalAngle - M_PI)
	);
	vec3 up = cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}

	float FoV = initialFoV;

	projection = perspective(radians(FoV), 1.3333f, 0.1f, 10000.0f);
	view = lookAt(position, position + direction, up);
	
	lastTime = currentTime;
}
