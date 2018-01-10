#include "Camera.hpp"
#include "Predefined.hpp"

using glm::cross;
using glm::perspective;
using glm::lookAt;
using glm::radians;

void Camera::computeMatricesFromInputs(GLFWwindow * window)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	const float centerOfWidth	= WIDTH / 2;
	const float centerOfHeight	= HEIGHT / 2;

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, centerOfWidth, centerOfHeight);

	horizontalAngle += mouseSpeed * float(centerOfWidth - xpos);
	verticalAngle	+= mouseSpeed * float(centerOfHeight - ypos);

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
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;

	projection = perspective(radians(FoV), RATIO_4_3, NEAR, FAR);
	view = lookAt(position, position + direction, up);
	
	lastTime = currentTime;
}