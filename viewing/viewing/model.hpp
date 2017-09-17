#ifndef model_h
#define model_h

#include "gl_framework.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include <vector>
#include <string>
#include <fstream>

#define BUFFER_OFFSET(offset) ((void *)(offset))

class Model {
	std::string name;
	std::vector<glm::vec4> points;
	std::vector<glm::vec4> colors;

	glm::mat4 scale_matrix;
	glm::mat4 rotation_matrix;
	glm::mat4 translation_matrix;

public:
	Model(std::string name);
	void setScaleMatrix(GLfloat sx, GLfloat sy, GLfloat sz);
	void setRotationMatrix(GLfloat rx, GLfloat ry, GLfloat rz);
	void setTranslationMatrix(GLfloat tx, GLfloat ty, GLfloat tz);

	const std::vector<glm::vec4>& getPoints() {return points;}
	const std::vector<glm::vec4>& getColors() {return colors;}
	std::vector<glm::vec4>* getTransformedPoints();
};

#endif