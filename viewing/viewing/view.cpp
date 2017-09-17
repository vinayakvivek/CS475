#include "view.hpp"
#include "model.hpp"

void View::initShadersGL() {
	std::string vertex_shader_file("shaders/vshader.glsl");
	std::string fragment_shader_file("shaders/fshader.glsl");

	std::vector<GLuint> shaderList;
	shaderList.push_back(viewing::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(viewing::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

	shaderProgram = viewing::createProgramGL(shaderList);
	glUseProgram(shaderProgram);
}

void View::initBuffersGL() {
	vPosition = glGetAttribLocation(shaderProgram, "vPosition");
	vColor = glGetAttribLocation(shaderProgram, "vColor");
	uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");

	num_vao = 2;
	num_vbo = 5;
	vao = new GLuint[num_vao];
	vbo = new GLuint[num_vbo];

	glGenVertexArrays(num_vao, vao);
	glGenBuffers(num_vbo, vbo);

	glBindVertexArray(vao[0]);
}

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth) {
	half_width = h_width;
	half_height = h_height;
	half_depth = h_depth;

	initShadersGL();
	initBuffersGL();

	ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);

	modelview_matrix = ortho_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));

	// addModel("models/hut-new", glm::vec3(0.5, 0.5, 0.5), glm::vec3(30.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	addModel("models/fan", glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 30.0, 60.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/chair", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 30.0, 30.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/goggles-new", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

	eye = glm::vec4(0.0, 0.0, 0.0, 1.0);
	lookAtPoint = glm::vec4(100.0, 0.0, 0.0, 1.0);
	glm::vec4 lookAtVector = (lookAtPoint - eye);
	upVec = glm::vec4(0.0, 1.0, 0.0, 0.0);

	// Frustum
	L = -100;
	R = 100;
	T = 100;
	B = -100;
  	N = 100;
  	F = 300;
}

void View::addModel(std::string name, glm::vec3 s, glm::vec3 r, glm::vec3 t) {
	Model *m = new Model(name);
	m->setScaleMatrix(s.x, s.y, s.z);
	m->setRotationMatrix(r.x, r.y, r.z);
	m->setTranslationMatrix(t.x, t.y, t.z);
	std::vector<glm::vec4> *t_points = m->getTransformedPoints();

	points.insert(points.end(), t_points->begin(), t_points->end());
	colors.insert(colors.end(), m->getColors().begin(), m->getColors().end());

	points_buffer_length = points.size() * 4 * sizeof(GLfloat);
	colors_buffer_length = colors.size() * 4 * sizeof(GLfloat);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_length));

	glBufferData(GL_ARRAY_BUFFER, points_buffer_length + colors_buffer_length, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, points_buffer_length, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, points_buffer_length, colors_buffer_length, &colors[0]);
}

void View::renderGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, points.size());
}