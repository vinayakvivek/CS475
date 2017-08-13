#version 400

in vec4 vPosition;
uniform mat4 uModelViewMatrix;

void main () {
	gl_Position = uModelViewMatrix * vPosition;
}
