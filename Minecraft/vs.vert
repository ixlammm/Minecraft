#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 iTexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;

void main() {
	texCoord = iTexCoord;
	gl_Position = projection * view * transform * vec4(pos, 1.0);
}