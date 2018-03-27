#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 xy;
layout(location = 2) in uint state;

out vec4 outColor;

uniform mat4 MVP;

void main()
{
	vec3 particleCenterWorldSpace = vec3(xy, 0.0f);
	vec3 vertexPositionWorldSpace =
		particleCenterWorldSpace
		+ vec3(1.0f, 0.0f, 0.0f) * vertex.x
		+ vec3(0.0f, 1.0f, 0.0f) * vertex.y;

	gl_Position = MVP * vec4(vertexPositionWorldSpace, 1.0f);
	if (state == 0) {
		outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == 1) {
		outColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}