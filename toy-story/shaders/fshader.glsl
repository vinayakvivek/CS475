#version 400

in vec4 color;
in vec2 tex_coord;

uniform sampler2D textureSampler;

out vec4 frag_colour;

void main () {
  vec4 baseColor = texture(textureSampler, tex_coord);
	frag_colour = baseColor;
}
