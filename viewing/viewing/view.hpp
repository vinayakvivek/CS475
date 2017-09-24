#ifndef view_h
#define view_h

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "model.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <vector>
#include <cmath>

#define BUFFER_OFFSET(offset) ((void *)(offset))

class View {

	// screen size
	// GLfloat half_width = 400, half_height = 300, half_depth = 500;
	GLfloat half_width;
	GLfloat half_height;
	GLfloat half_depth;

	// models setup
	std::vector<glm::vec4> points;
	std::vector<glm::vec4> colors;

	GLuint points_buffer_length;
	GLuint colors_buffer_length;

	// VCS setup
	glm::vec3 eye;
	glm::vec3 lookAtPoint;
	glm::vec3 upVec;

	// Frustum
	GLfloat L, R, T, B;
  	GLfloat N, F;

  	std::vector<glm::vec4> frustum_points;
	std::vector<glm::vec4> frustum_colors;
	std::vector<GLuint> frustum_indices;

	GLuint num_vao;
	GLuint num_vbo;
	GLuint num_eab;
	GLuint shaderProgram;
	GLuint *vbo, *vao, *veo;

	// shader variables
	GLint vPosition, vColor;
	GLint uModelMatrix, uOrthoMatrix;
	GLint uPerspectiveDivide;
	GLint uWindowLimits;		// (R, L, T, B)
	GLint uToDCS;

	// transformation matrices
	glm::mat4 ortho_matrix;
	glm::mat4 rotation_matrix;
	GLfloat xrot, yrot, zrot;
	glm::mat4 model_matrix;
	GLuint perspective_divide;
	GLuint to_dcs;
	glm::vec4 window_limits;

	glm::mat4 wcs_to_vcs_matrix;
	glm::mat4 vcs_to_wcs_matrix;

	glm::mat4 vcs_to_ccs_matrix;
	glm::mat4 ccs_to_vcs_matrix;

	/**
	 * CS - coordinate system
	 * 	0 -> WCS
	 *	1 -> VCS
	 *	2 -> CCS
	 * 	3 -> NDCS
	 * 	4 -> DCS
	 */
	GLuint CS;

	void initShadersGL();
	void initBuffersGL();
	void addViewFrustum();

	void calcStageTransformations();
	void calcPointsNDCS();
	void test();
public:
	View(GLfloat h_width, GLfloat h_height, GLfloat h_depth);
	void addModel(std::string name, glm::vec3 s, glm::vec3 r, glm::vec3 t);
	void renderGL();

	void updateRotationMatrix(GLuint axis, GLfloat angle);
	void updateCS(int val);
};


#endif