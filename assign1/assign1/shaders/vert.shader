#version 400

in vec4 vPosition;
uniform mat4 uModelViewMatrix;

in vec4 vColor;
out vec4 color_from_vshader;

void main () {
	gl_Position = uModelViewMatrix * vPosition;
	color_from_vshader = vColor;
}
