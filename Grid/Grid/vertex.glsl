#version 330 core

#define AIR 0.0f
#define FLUID 1.0f
#define BOUNDARY 2.0f

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 xy;
layout(location = 2) in float state;

out vec4 color;

uniform mat4 MVP;
uniform float size;

void main()
{
	vec2 position = xy + vec2(vertex.x * size, vertex.y * size);

	gl_Position = MVP * vec4(position, 0.0f, 1.0f);

	if (state == AIR) {
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == FLUID) {
		color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}