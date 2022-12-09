#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 iTexCoord;
layout(location = 2) in vec3 iNormal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 normal;

void main() {
	texCoord = iTexCoord;
	normal = iNormal;
	gl_Position = projection * view * transform * vec4(pos, 1.0);
}