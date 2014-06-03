#version 400 core

uniform mat4 vProjection;
uniform mat4 vView;
uniform mat4 vModel;

in vec3 vPosition;
in vec3 vColor;

out vec3 fColor;

void main() {
	gl_Position = vProjection * vView * vModel * vec4( vPosition, 1.0 );
	gl_Position = gl_Position / gl_Position.w;
	fColor = vColor;
}