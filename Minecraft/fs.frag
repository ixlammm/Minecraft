#version 330 core

uniform sampler2D text;

in vec2 texCoord;

out vec4 fragColor;

uniform vec3 lightDir;
uniform vec3 ambient;

in vec3 normal;

void main() {
	float up = dot(normal, vec3(0, 1, 0));
	float dfb = (int(texCoord.y * 4096) % 16) / 16.0 * (1 - up) + up;
	fragColor = texture(text, texCoord) * (max((max(0, dot(normal, lightDir) + ambient.x)), 1) - (1 - dfb) * 0.2);
}