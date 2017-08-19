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
	glm::vec4(-100.0f,  -100.0f,  100.0f, 1.0f),
	glm::vec4(-100.0f, 100.0f,  100.0f, 1.0f),
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

const GLuint num_vao = 2, num_vbo = 2;
GLuint shaderProgram;
GLuint vbo[num_vao], vao[num_vbo];
GLint vPosition, vColor;
GLint uModelViewMatrix;

// rotation parameters
GLfloat theta = 0.1;
GLfloat xrot = 0.0, yrot = 0.0, zrot = 0.0;

// position parameters
GLfloat delta = 10.0;
GLfloat xpos = 0.0, ypos = 0.0, zpos = 0.0;

glm::mat4 translate_matrix;
glm::mat4 rotation_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
	

#endif /* model_h */
