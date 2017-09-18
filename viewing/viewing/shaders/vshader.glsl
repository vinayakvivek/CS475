#version 400

in vec4 vPosition;
in vec4 vColor;
uniform mat4 uModelMatrix;
uniform mat4 uOrthoMatrix;
uniform int uPerspectiveDivide;
out vec4 color;

void main () {
	vec4 v;
	v = uModelMatrix * vPosition;

	if (uPerspectiveDivide == 1) {
		v = vec4(v.x/v.w, v.y/v.w, v.z/v.w, 1.0);
	}

	v = uOrthoMatrix * v;
	gl_Position = v;
	gl_PointSize = 10.0;
	color = vColor;
}
