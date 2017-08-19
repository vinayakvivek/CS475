#version 400

in vec4 vPosition;
in vec4 vColor;
uniform mat4 uModelViewMatrix;
out vec4 color;

void main () {
	gl_Position = uModelViewMatrix * vPosition;
	gl_PointSize = 10.0;
	color = vColor;
}
