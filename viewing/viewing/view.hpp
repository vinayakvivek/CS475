#ifndef view_h
#define view_h

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

#define BUFFER_OFFSET(offset) ((void *)(offset))


class View {

	// screen size
	// GLfloat half_width = 400, half_height = 300, half_depth = 500;
	GLfloat half_width;
	GLfloat half_height;
	GLfloat half_depth;

	std::vector<glm::vec4> points;
	std::vector<glm::vec4> colors;

	GLuint points_buffer_length;
	GLuint colors_buffer_length;

	GLuint num_vao;
	GLuint num_vbo;
	GLuint shaderProgram;
	GLuint *vbo, *vao;
	GLint vPosition, vColor;
	GLint uModelViewMatrix;

	glm::mat4 ortho_matrix;
	glm::mat4 modelview_matrix;

	void initShadersGL();
	void initBuffersGL();
public:
	View(GLfloat h_width, GLfloat h_height, GLfloat h_depth);
	void addModel(std::string name, glm::vec3 s, glm::vec3 r, glm::vec3 t);
	void renderGL();
};


#endif