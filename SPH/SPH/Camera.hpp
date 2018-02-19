#pragma once

#include <GL/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using glm::mat4;
using glm::vec3;

class Camera
{
public :
	Camera(const vec3& position = vec3()) :
		position(position),
		horizontalAngle(3.14f), verticalAngle(0.0f), initialFoV(45.0f),
		speed(3.0f), mouseSpeed(0.005f) {}
	
	void computeMatricesFromInputs(GLFWwindow* window);

	mat4 getView() const { return view; }
	mat4 getProjection() const { return projection; }

private :
	mat4 view;
	mat4 projection;
	vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;
};