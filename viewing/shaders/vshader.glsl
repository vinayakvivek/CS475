#version 400

in vec4 vPosition;
in vec4 vColor;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform int uPerspectiveDivide;
uniform vec4 uWindowLimits;		// (R, L, T, B)
uniform int uToDCS;

out vec4 color;

void main () {
	vec4 v;
	v = uModelMatrix * vPosition;

	if (uPerspectiveDivide == 1) {
		v = v / v.w;
	}

	if (uToDCS == 1) {
		float R = uWindowLimits.x;
		float L = uWindowLimits.y;
		float T = uWindowLimits.z;
		float B = uWindowLimits.w;

		float x = 0.5 * (v.x + 1) * (R - L) + L;
		float y = 0.5 * (v.y + 1) * (T - B) + B;
		float z = 0.5 * (v.z + 1);
		v = vec4(x, y, z, 1.0);
	}

	v = uViewMatrix * v;
	gl_Position = v;
	gl_PointSize = 10.0;
	color = vColor;
}
