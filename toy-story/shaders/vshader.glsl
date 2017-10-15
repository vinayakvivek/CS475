#version 400

in vec4 vPosition;
in vec4 vColor;
in vec2 vTexture;
in vec4 vNormal;

uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uNormalMatrix;

out vec4 color;

void main () {
	gl_Position = uViewMatrix * uModelMatrix * vPosition;
	color = vColor;
}
