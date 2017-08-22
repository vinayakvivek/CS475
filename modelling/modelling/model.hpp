#ifndef model_h
#define model_h

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define BUFFER_OFFSET(offset) ((void *)(offset))


GLfloat half_width = 400, half_height = 300, half_depth = 300;

GLuint points_buffer_length = 0;
GLuint colors_buffer_length = 0;

std::vector<glm::vec4> points({
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	glm::vec4(-100.0f,  -100.0f,  0.0f, 1.0f),
	glm::vec4(-100.0f, 100.0f,  0.0f, 1.0f),
});
std::vector<glm::vec4> colors({
	glm::vec4(0.5f, 0.0f, 0.5f, 1.0f),
	glm::vec4(0.3f,  0.5f,  0.8f, 1.0f),
	glm::vec4(0.4f, 0.7f,  0.2f, 1.0f),
});


GLuint temp_points_buffer_length = 0;
GLuint temp_colors_buffer_length = 0;
std::vector<glm::vec4> temp_points;
std::vector<glm::vec4> temp_colors;

modelling::state_enum state = modelling::s_model;

const GLuint num_vao = 4, num_vbo = 4, num_eab = 2;
GLuint shaderProgram;
GLuint vbo[num_vao], vao[num_vbo], eab[num_eab];
GLint vPosition, vColor;
GLint uModelViewMatrix;

// rotation parameters
GLfloat theta = 0.1;
GLfloat xrot = 0.0, yrot = 0.0, zrot = 0.0;

// position parameters
GLfloat delta = 10.0;
GLfloat xpos = 0.0, ypos = 0.0, zpos = 0.0;

glm::mat4 translate_matrix;
glm::mat4 translate_centroid_matrix;
glm::mat4 plane_rotation_matrix;
glm::mat4 rotation_matrix;
glm::mat4 model_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;

glm::vec4 centroid = glm::vec4(0.0, 0.0, 0.0, 1.0);

GLboolean show_planes = true;
GLboolean show_frontal_plane = true;

// xy, yz, zx planes
glm::vec4 plane_points[] = {
	// XY
	glm::vec4(-half_width, half_height, 0.0, 1.0),
	glm::vec4(-half_width, -half_height, 0.0, 1.0),
	glm::vec4(half_width, -half_height, 0.0, 1.0),
	glm::vec4(half_width, half_height, 0.0, 1.0),
	
	// YZ
	glm::vec4(0.0, half_height, half_depth, 1.0),
	glm::vec4(0.0, -half_height, half_depth, 1.0),
	glm::vec4(0.0, -half_height, -half_depth, 1.0),
	glm::vec4(0.0, half_height, -half_depth, 1.0),
	
	// ZX
	glm::vec4(-half_width, 0.0, -half_depth, 1.0),
	glm::vec4(-half_width, 0.0, half_depth, 1.0),
	glm::vec4(half_width, 0.0, half_depth, 1.0),
	glm::vec4(half_width, 0.0, -half_depth, 1.0),
};
glm::vec4 xy_color = glm::vec4(0.8, 0.8, 0.8, 0.5);
glm::vec4 yz_color = glm::vec4(0.8, 0.5, 0.7, 0.5);
glm::vec4 zx_color = glm::vec4(0.6, 0.5, 0.9, 0.5);
glm::vec4 plane_colors[] = {
	xy_color, xy_color, xy_color, xy_color,
	yz_color, yz_color, yz_color, yz_color,
	zx_color, zx_color, zx_color, zx_color,
};
GLuint plane_indices[] = {
	0, 1, 3,
	3, 1, 2,
	
	4, 5, 6,
	4, 6, 7,
	
	8, 9, 10,
	8, 10, 11,
};


// current frontal plane
glm::vec4 frontal_plane_points[] = {
	glm::vec4(-half_width, half_height, 0.0, 1.0),
	glm::vec4(-half_width, -half_height, 0.0, 1.0),
	glm::vec4(half_width, -half_height, 0.0, 1.0),
	glm::vec4(half_width, half_height, 0.0, 1.0),
};
glm::vec4 f_plane_color = glm::vec4(1.0, 1.0, 1.0, 0.5);
glm::vec4 frontal_plane_colors[] = {
	f_plane_color, f_plane_color, f_plane_color, f_plane_color,
};
GLuint frontal_plane_indices[] = {
	0, 1, 3,
	3, 1, 2,
};

#endif /* model_h */
