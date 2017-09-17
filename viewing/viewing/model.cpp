#include "model.hpp"

Model::Model(std::string name) {
	this->name = name;
	scale_matrix = glm::mat4(1.0f);
	rotation_matrix = glm::mat4(1.0f);
	translation_matrix = glm::mat4(1.0f);

	// file name will be <name>.raw
	std::string file_name = name + ".raw";

	std::ifstream model_file;
	model_file.open(file_name, std::ios::in);

	if (!model_file.is_open()) {
		std::cout << "could not load model :( \n";
		return;
	}

	GLfloat x, y, z, r, g, b;
	while (model_file >> x >> y >> z >> r >> g >> b) {
		points.push_back(glm::vec4(x, y, z, 1.0));
		colors.push_back(glm::vec4(r, g, b, 1.0));
	}

	model_file.close();
}

void Model::setScaleMatrix(GLfloat sx, GLfloat sy, GLfloat sz) {
	/*
		[sx 0  0  0
		 0  sy 0  0
		 0  0  sz 0
		 0  0  0  1]

		// glm::mat4 is initialized in column major order
	*/
	scale_matrix = glm::mat4(sx, 0.0, 0.0, 0.0,
	                         0.0, sy, 0.0, 0.0,
	                         0.0, 0.0, sz, 0.0,
	                         0.0, 0.0, 0.0, 1.0);
}

void Model::setRotationMatrix(GLfloat rx, GLfloat ry, GLfloat rz) {
	/*
		[]

		// glm::mat4 is initialized in column major order
	*/
	rotation_matrix = glm::mat4(1.0, 0.0, 0.0, 0.0,
	                            0.0, 1.0, 0.0, 0.0,
	                            0.0, 0.0, 1.0, 0.0,
	                            0.0, 0.0, 0.0, 1.0);
}

void Model::setTranslationMatrix(GLfloat tx, GLfloat ty, GLfloat tz) {
	/*
		[1  0  0  tx
		 0  1  0  ty
		 0  0  1  tz
		 0  0  0  1 ]

		// glm::mat4 is initialized in column major order
	*/
	translation_matrix = glm::mat4(1.0, 0.0, 0.0, 0.0,
	                               0.0, 1.0, 0.0, 0.0,
	                               0.0, 0.0, 1.0, 0.0,
	                               tx, ty, tz, 1.0);
}

std::vector<glm::vec4>* Model::getTransformedPoints() {

	glm::mat4 model_matrix = translation_matrix * scale_matrix * rotation_matrix;

	std::vector<glm::vec4> *t_points = new std::vector<glm::vec4>();
	for (glm::vec4 p : points) {
		t_points->push_back(model_matrix * p);
	}

	return t_points;
}