#version 330 core

#define GRID_CHANNEL 1
#define PARTICLE_CHANNEL 2

in vec2 UV;

out vec4 pixelColor;

uniform int channel;
uniform sampler2D myTextureSampler;

void main()
{
	if (channel == GRID_CHANNEL) {
		pixelColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (channel == PARTICLE_CHANNEL) {
		pixelColor = texture(myTextureSampler, UV) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
}