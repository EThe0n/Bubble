#pragma once

#include <GL/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm\glm.hpp>

class Camera
{
public :
	Camera(float x, float y, float z);

	/**
		@brief : Update Camera status
	 */
	void update(GLFWwindow* window, float width, float height, float deltaTime);

public :
	glm::mat4 vp;
	glm::mat4 view;
	glm::mat4 projection;

private :
	glm::vec3 position;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 direction;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
	
private :
	void move(GLFWwindow* window, float deltaTime);
	void rotate(float dx, float dy);
};