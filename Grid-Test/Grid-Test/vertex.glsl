#version 330 core

#define GRID_CHANNEL 1
#define PARTICLE_CHANNEL 2

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec4 xyzs;

out vec2 UV;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform int channel;
uniform mat4 MVP;

void main()
{
	if (channel == GRID_CHANNEL) {
		gl_Position = MVP * vec4(Vertex, 1.0f);
	}
	else if (channel == PARTICLE_CHANNEL) {
		float particleSize = xyzs.w;
		vec3 particleCenter_worldSpace = xyzs.xyz;

		vec3 vertexPosition_worldspace =
			particleCenter_worldSpace
			+ CameraRight_worldspace * Vertex.x * particleSize
			+ CameraUp_worldspace * Vertex.y * particleSize;

		// Output position of the vertex
		gl_Position = MVP * vec4(vertexPosition_worldspace, 1.0f);

		// UV of the vertex. No special space for this one.
		UV = Vertex.xy + vec2(0.5, 0.5);
	}
}