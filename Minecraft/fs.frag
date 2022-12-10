#version 330 core

uniform sampler2D text;

in vec2 texCoord;

out vec4 fragColor;

uniform vec3 lightDir;
uniform vec3 ambient;
uniform vec3 camPos;

in vec3 normal;
in vec4 fragPos;

void main() {
	float dist = min(0.8, max(1, length((camPos - fragPos.xyz) * vec3(1, 0, 1)) / 20.0));
	float up = max(0, dot(normal, vec3(0, 1, 0)));
	float fs = max(0.6, (dot(normal, vec3(1, 0, 1) * lightDir) + 1) / 2);
	float dfb = (int(texCoord.y * 4096) % 256 / 256.0) * (1 - up) + up;
	float final = min(1, fs + ambient.x - (1 - dfb) * dist);
	fragColor = texture(text, texCoord) * final;
}