#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;

Camera::Camera(float x, float y, float z)
{
	position = vec3(x, y, z);
	horizontalAngle = 3.141f;
	verticalAngle = 0.0f;
	initialFoV = glm::radians(45.0f);
	speed = 3.0f;
	mouseSpeed = 0.005f;
}

void Camera::update(GLFWwindow * window, float width, float height, float deltaTime)
{
	register double xpos, ypos;
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;
	
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, halfWidth, halfHeight);

	move(window, deltaTime);
	rotate(halfWidth - (float)xpos, halfHeight - (float)ypos);

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(initialFoV, 1.3333f, 0.1f, 10000.0f);

	// Camera matrix
	view = glm::lookAt(
		position,				// Camera is here
		position + direction,	// and looks here : at the same position, plus "direction"
		up						// Head is up (set to 0,-1,0 to look upside-down)
	);
	vp = projection * view;
}

void Camera::move(GLFWwindow* window, float deltaTime)
{
	const float distance = deltaTime * speed;
	
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * distance;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * distance;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * distance;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * distance;
	}
}

void Camera::rotate(float dx, float dy)
{
	//horizontalAngle += mouseSpeed * dx;
	//verticalAngle += mouseSpeed * dy;

	direction = vec3(
		cosf(verticalAngle) * sinf(horizontalAngle),
		sinf(verticalAngle),
		cosf(verticalAngle) * cosf(horizontalAngle)
	);

	right = vec3(
		sinf(horizontalAngle - 1.571f),
		0.0f,
		cosf(horizontalAngle - 1.571f)
	);
	
	up = glm::cross(right, direction);
}
