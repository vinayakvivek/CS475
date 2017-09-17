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
				(1    0      0    0)
		Rx(q) = (0  cos q  sin q  0)
		        (0 -sin q  cos q  0)
		        (0    0     0     1)

		        (cos q  0  -sin q   0)
		Ry(q) = (0      1    0      0)
		        (sin q  0  cos q    0)
		        (0      0    0     1)

		        ( cos q  sin q  0  0)
		Rz(q) = (-sin q  cos q  0  0)
		        ( 0        0    1  0)
		        ( 0        0    0  1)

		// glm::mat4 is initialized in column major order
	*/
	rx = deg_to_rad(-rx);
	ry = deg_to_rad(-ry);
	rz = deg_to_rad(-rz);

	GLfloat sinx = sin(rx), cosx = cos(rx);
	GLfloat siny = sin(ry), cosy = cos(ry);
	GLfloat sinz = sin(rz), cosz = cos(rz);

	glm::mat4 xrot = glm::mat4(1.0, 0.0, 0.0, 0.0,
			                   0.0, cosx, -sinx, 0.0,
			                   0.0, sinx, cosx, 0.0,
			                   0.0, 0.0, 0.0, 1.0);

	glm::mat4 yrot = glm::mat4(cosy, 0.0, siny, 0.0,
	                           0.0, 1.0, 0.0, 0.0,
	                           -siny, 0.0, cosy, 0.0,
	                           0.0, 0.0, 0.0, 1.0);

	glm::mat4 zrot = glm::mat4(cosz, -sinz, 0.0, 0.0,
	                           sinz, cosz, 0.0, 0.0,
	                           0.0, 0.0, 1.0, 0.0,
	                           0.0, 0.0, 0.0, 1.0);

	rotation_matrix = zrot * yrot * xrot;
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

void Model::move_centroid_to_origin() {
	glm::vec3 p = glm::vec3(0.0, 0.0, 0.0);
	int n = points.size();

	for (glm::vec4 point : points) {
		p += glm::vec3(point);
	}
	p /= n;

	glm::vec4 centroid(p, 0.0);

	for (int i = 0; i < n; ++i) {
		points[i] -= centroid;
	}
}

std::vector<glm::vec4>* Model::getTransformedPoints() {
	move_centroid_to_origin();
	glm::mat4 model_matrix = translation_matrix * scale_matrix * rotation_matrix;

	std::vector<glm::vec4> *t_points = new std::vector<glm::vec4>();
	for (glm::vec4 p : points) {
		t_points->push_back(model_matrix * p);
	}

	return t_points;
}