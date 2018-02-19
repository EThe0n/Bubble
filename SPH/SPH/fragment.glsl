#version 330 core

#define GRID_CHANNEL 1
#define PARTICLE_CHANNEL 2

in vec2 UV;

out vec4 pixelColor;

uniform int channel;
uniform sampler2D myTextureSampler;

void main()
{
	if (channel == PARTICLE_CHANNEL) {
		vec4 texel = texture(myTextureSampler, UV);
		if (texel.a < 0.5) {
			discard;
		}
		pixelColor = texel * vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else if (channel == GRID_CHANNEL) {
		pixelColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}