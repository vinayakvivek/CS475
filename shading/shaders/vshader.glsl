#version 400

in vec4 vPosition;
// in vec4 vColor;
in vec4 vNormal;
in vec2 vTex;
in vec3 vTangent;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uNormalMatrix;

out vec2 tex;
// out vec4 color;
// out vec4 normal;
out vec4 position;
out vec3 lightVec;

void main () {
	gl_Position = uViewMatrix * uModelMatrix * vPosition;
	// color = vColor;
	// normal = uNormalMatrix * normalize(vNormal);
	position = uModelMatrix * vPosition;
	tex = vTex;

	vec3 tangent = normalize((uModelMatrix * vec4(vTangent, 1.0)).xyz);
	vec3 normal = normalize((uNormalMatrix * normalize(vNormal)).xyz);
	vec3 binormal = normalize(cross(normal, tangent));
	mat3 TBNMatrix = mat3(tangent, binormal, normal);

	vec3 lightPosition = vec3(400.0, 400.0, 400.0);
	lightVec = lightPosition - position.xyz;
	lightVec *= TBNMatrix;
}
