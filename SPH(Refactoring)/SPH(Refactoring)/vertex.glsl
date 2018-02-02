#version 330 core

#define GRID_CHANNEL 1
#define PARTICLE_CHANNEL 2

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 xy;

out vec2 UV;

uniform int channel;
uniform mat4 MVP;
uniform float particleSize;

void main()
{
	if (channel == PARTICLE_CHANNEL) {
		vec3 particleCenter_worldSpace = vec3(xy, 0.0f);

		vec3 vertexPosition_worldspace =
			particleCenter_worldSpace
			+ vec3(1.0f, 0.0f, 0.0f) * vertex.x * particleSize
			+ vec3(0.0f, 1.0f, 0.0f) * vertex.y * particleSize;

		// Output position of the vertex
		gl_Position = MVP * vec4(vertexPosition_worldspace, 1.0f);

		// UV of the vertex. No special space for this one.
		UV = vertex.xy + vec2(0.5f, 0.5f);
	}
	else if (channel == GRID_CHANNEL) {
		gl_Position = MVP * vec4(vertex, 0.0f, 1.0f);
	}
}