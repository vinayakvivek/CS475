#include "view.hpp"

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
	num_eab = 1;
	vao = new GLuint[num_vao];
	vbo = new GLuint[num_vbo];
	veo = new GLuint[num_eab];

	glGenVertexArrays(num_vao, vao);
	glGenBuffers(num_vbo, vbo);
	glGenBuffers(num_eab, veo);
}

void View::addViewFrustum() {

	/*
		n = −(A−E) / |A−E|
		u = (v_up × n) / |v_up × n|
		v = n × u

		x -> u
		y -> v
		z -> n

		M = R * T where T is a
		translation matrix by EYE, and
		R is rotation matrix whose columns
		are n, u, and v:

	*/

	glm::vec4 n(glm::normalize(eye - lookAtPoint), 0.0);
	glm::vec4 u(glm::normalize(glm::cross(upVec, glm::vec3(n))), 0.0);
	glm::vec4 v(glm::cross(glm::vec3(n), glm::vec3(u)), 0.0);

	glm::mat4 rot(u, v, n, glm::vec4(0.0, 0.0, 0.0, 1.0));
	glm::mat4 trans(glm::vec4(1.0, 0.0, 0.0, 0.0),
	            glm::vec4(0.0, 1.0, 0.0, 0.0),
	            glm::vec4(0.0, 0.0, 1.0, 0.0),
	            glm::vec4(eye, 1.0));

	wcs_to_vcs_matrix = trans * rot;
	// glm::mat4 vcs_to_wcs_matrix = glm::inverse(wcs_to_vcs_matrix);

	// line parameter (for finding points of far plane)
	GLfloat t = F / N;
	frustum_points = std::vector<glm::vec4>({
		// for projectors
		glm::vec4(0.0, 0.0, 0.0, 1.0),
		glm::vec4(L, T, -N, 1.0),
		glm::vec4(L, B, -N, 1.0),
		glm::vec4(R, B, -N, 1.0),
		glm::vec4(R, T, -N, 1.0),

		// near plane
		glm::vec4(L, T, -N, 1.0),
		glm::vec4(L, B, -N, 1.0),
		glm::vec4(R, B, -N, 1.0),
		glm::vec4(R, T, -N, 1.0),

		// far plane
		glm::vec4(L*t, T*t, -F, 1.0),
		glm::vec4(L*t, B*t, -F, 1.0),
		glm::vec4(R*t, B*t, -F, 1.0),
		glm::vec4(R*t, T*t, -F, 1.0),
	});

	frustum_colors = std::vector<glm::vec4>({
		glm::vec4(1.0, 0.0, 1.0, 1.0),
		glm::vec4(1.0, 0.0, 1.0, 1.0),
		glm::vec4(1.0, 0.0, 1.0, 1.0),
		glm::vec4(1.0, 0.0, 1.0, 1.0),
		glm::vec4(1.0, 0.0, 1.0, 1.0),

		// near plane
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
		glm::vec4(0.0, 1.0, 1.0, 1.0),
	});

	frustum_indices = std::vector<GLuint>({
		// projectors
		0, 1, 0, 2, 0, 3, 0, 4,

		5, 6, 6, 7, 7, 8, 8, 5,
		9, 10, 10, 11, 11, 12, 12, 9,

		5, 9, 6, 10, 7, 11, 8, 12
	});

	GLuint f_buffer_len = frustum_points.size() * 4 * sizeof(GLfloat);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(f_buffer_len));

	glBufferData(GL_ARRAY_BUFFER, 2 * f_buffer_len, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, f_buffer_len, &frustum_points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, f_buffer_len, f_buffer_len, &frustum_colors[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, frustum_indices.size() * sizeof(GLuint), &frustum_indices[0], GL_STATIC_DRAW);
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

	addModel("models/hut-new", glm::vec3(0.5, 0.5, 0.5), glm::vec3(30.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/fan", glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 30.0, 60.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/chair", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 30.0, 30.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/goggles-new", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

	eye = glm::vec3(200.0, 0.0, -200.0);
	lookAtPoint = glm::vec3(0.0, 0.0, 0.0);
	upVec = glm::vec3(0.0, 0.0, 1.0);
	upVec = glm::normalize(upVec);

	// Frustum
	L = -50;
	R = 50;
	T = 50;
	B = -50;
  	N = 200;
  	F = 500;

  	addViewFrustum();
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
	modelview_matrix = ortho_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	glDrawArrays(GL_TRIANGLES, 0, points.size());

	glBindVertexArray(vao[1]);
	modelview_matrix = ortho_matrix * wcs_to_vcs_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	glDrawElements(GL_LINES, frustum_indices.size(), GL_UNSIGNED_INT, 0);
}