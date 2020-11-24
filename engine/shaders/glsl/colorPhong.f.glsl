#version 410 core

in vec3 color;
out vec4 fragColorOut;

void main() {
	fragColorOut = vec4(color, 1);
}
