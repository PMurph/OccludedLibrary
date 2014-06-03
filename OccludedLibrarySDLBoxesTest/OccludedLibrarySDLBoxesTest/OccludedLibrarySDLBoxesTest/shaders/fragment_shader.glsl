#version 400 core

in vec3 fColor;

out vec4 finalColor;

void main() {
	finalColor = vec4( fColor, 1.0 );
}