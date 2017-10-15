#version 400

in vec4 color;
in vec2 tex_coord;
in vec4 normal;
in vec4 position;

uniform sampler2D textureSampler;
uniform vec4 uCameraPosition;
uniform vec4 uLightPositions[2];
uniform vec4 uSpotLightPosition[2];
uniform bool uLightsState[3];

out vec4 out_color;

void main () {
  vec4 base_color = texture(textureSampler, tex_coord);
  out_color = vec4(0.0);

  // Defining material properties
  vec4 diffuse = vec4(1.0, 1.0, 0.0, 1.0);
  vec4 ambient = vec4(0.1, 0.0, 0.1, 1.0);
  vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
  float shininess = 5.0;
  vec4 spec = vec4(0.0);

  vec3 norm = normalize(normal.xyz);

  // ---------------------------

  // Light 1
  if (uLightsState[0]) {
    vec4 lightPosition = uLightPositions[1];
    vec3 lightVector = lightPosition.xyz - position.xyz;
    // optional: attenuation
    float dist = length(lightVector);
    float attenuation = 1.0 / (1.0 + 0.0001 * dist + 0.000001 * dist * dist);

    lightVector = normalize(lightVector);

    float nxDir = max(0.0, dot(norm, lightVector));
    out_color += base_color * diffuse * nxDir;
    vec4 cameraPosition = uCameraPosition;

    if (nxDir != 0) {
      vec3 cameraVector = normalize(cameraPosition.xyz - position.xyz);
      vec3 halfVector = normalize(lightVector + cameraVector);
      float nxHalf = max(0.0, dot(norm, halfVector));
      float specularPower = pow(nxHalf, shininess);

      out_color += base_color * specular * specularPower;
    }
    out_color *= attenuation;
  }

  // ----------------------------
  out_color += (base_color * ambient);
}
