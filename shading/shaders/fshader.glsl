#version 400

// in vec4 color;
// in vec4 normal;
in vec4 position;
in vec2 tex;
in vec3 lightVec;

uniform sampler2D textureSampler;
uniform sampler2D normalMapSampler;

out vec4 frag_colour;

void main () {
  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 0.0, 1.0);
  vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
  vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
  float shininess = 5.0;
  vec4 spec = vec4(0.0);

  vec4 final_color = vec4(0.0);
  vec4 cameraPosition = vec4(0.0, 0.0, 500.0, 1.0);

  // frag_colour = vec4(1.0, 0.4, 0.1, 1.0);
  vec3 norm = normalize(texture(normalMapSampler, tex).rgb * 2.0 - 1);
  vec4 baseColor = texture(textureSampler, tex);

  float dist = length(lightVec);
  float attenuation = 1.0 / (1.0 + 0.0001 * dist + 0.000001 * dist * dist);

  vec3 lightVector = normalize(lightVec);
  float nxDir = max(0.0, dot(norm, lightVector));
  final_color += baseColor * diffuse * nxDir;

  if (nxDir != 0) {
    vec3 cameraVector = normalize(cameraPosition.xyz - position.xyz);
    vec3 halfVector = normalize(lightVector + cameraVector);
    float nxHalf = max(0.0, dot(norm, halfVector));
    float specularPower = pow(nxHalf, shininess);

    final_color += baseColor * specular * specularPower;
  }

  final_color = final_color * attenuation;
  final_color += baseColor * ambient;

  frag_colour = final_color;


  // vec3 norm = normalize(normal.xyz);


  // ---------------------------

  // Light 1
  // vec4 lightPosition = vec4(400.0, 400.0, 400.0, 1.0);
  // vec3 lightVector = lightPosition.xyz - position.xyz;
  // optional: attenuation
  // float dist = length(lightVector);
  // float attenuation = 1.0 / (1.0 + 0.0001 * dist + 0.000001 * dist * dist);

  // // lightVector = normalize(lightVector);

  // float nxDir = max(0.0, dot(norm, lightVector));
  // final_color += frag_colour * diffuse * nxDir;
  // vec4 cameraPosition = vec4(0.0, 0.0, 500.0, 1.0);

  // if (nxDir != 0) {
  //   vec3 cameraVector = normalize(cameraPosition.xyz - position.xyz);
  //   vec3 halfVector = normalize(lightVector + cameraVector);
  //   float nxHalf = max(0.0, dot(norm, halfVector));
  //   float specularPower = pow(nxHalf, shininess);

  //   final_color += frag_colour * specular * specularPower;
  // }

  // ---------------------------

  // // Light 2
  // lightPosition = vec4(0.0, -210.0, 0.0, 1.0);
  // lightVector = lightPosition.xyz - position.xyz;
  // // optional: attenuation

  // lightVector = normalize(lightVector);

  // nxDir = max(0.0, dot(norm, lightVector));
  // final_color += frag_colour * diffuse * nxDir;

  // ----------------------------

  // final_color = final_color * attenuation;
  // final_color += frag_colour * ambient;

  // frag_colour = final_color;
}