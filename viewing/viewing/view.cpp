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
	uModelMatrix = glGetUniformLocation(shaderProgram, "uModelMatrix");
	uOrthoMatrix = glGetUniformLocation(shaderProgram, "uOrthoMatrix");
	uPerspectiveDivide = glGetUniformLocation(shaderProgram, "uPerspectiveDivide");
	uWindowLimits = glGetUniformLocation(shaderProgram, "uWindowLimits");
	uToDCS = glGetUniformLocation(shaderProgram, "uToDCS");

	num_vao = 3;
	num_vbo = 3;
	num_eab = 1;
	vao = new GLuint[num_vao];
	vbo = new GLuint[num_vbo];
	veo = new GLuint[num_eab];

	glGenVertexArrays(num_vao, vao);
	glGenBuffers(num_vbo, vbo);
	glGenBuffers(num_eab, veo);

	glEnable(GL_PROGRAM_POINT_SIZE);
}

void View::calcStageTransformations() {
	/*
		VCS to WCS

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

	vcs_to_wcs_matrix = trans * rot;
	wcs_to_vcs_matrix = glm::inverse(vcs_to_wcs_matrix);


	/*
		VCS to CCS

		A = Nm * Sc * Sh;
					[	2n / (r-l)		0		(r+l) / (r-l)		0		]
		v_ccs   =  	[		0		2n / (t-b)	(t+b) / (t-b)		0		] * v_vcs
					[		0			0		(f+n) / (n-f)	2fn / (n-f)	]
					[		0			0			-1				0		]

	*/

	vcs_to_ccs_matrix = glm::mat4(glm::vec4(2*N/(R-L), 0.0, 0.0, 0.0),
	                              glm::vec4(0.0, 2*N/(T-B), 0.0, 0.0),
	                              glm::vec4((R+L)/(R-L), (T+B)/(T-B), -(F+N)/(F-N), -1.0),
	                              glm::vec4(0.0, 0.0, -2*F*N/(F-N), 0.0));
	ccs_to_vcs_matrix = glm::inverse(vcs_to_ccs_matrix);

	/*
		NDCS -> DCS

		x'  = 	0.5 * (x+1) * (R - L) + L
		y'  = 	0.5 * (y+1) * (T - B) + B
		z'  = 	0.5 * (z+1)
	*/
}

void View::addViewFrustum() {

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

	for (int i = 0; i < frustum_points.size(); ++i) {
		frustum_points[i] = vcs_to_wcs_matrix * frustum_points[i];
	}

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

void View::addEye() {
	eye_point = glm::vec4(eye, 1.0);
	eye_color = glm::vec4(1.0, 0.0, 0.0, 1.0);	// RED

	GLuint buffer_len = sizeof(glm::vec4);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_len));

	glBufferData(GL_ARRAY_BUFFER, 2 * buffer_len, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_len, &eye_point);
	glBufferSubData(GL_ARRAY_BUFFER, buffer_len, buffer_len, &eye_color);
}

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth,
		   glm::vec3 *vcs, GLfloat *frustum) {
	half_width = h_width;
	half_height = h_height;
	half_depth = h_depth;

	xrot = 0.0;
	yrot = 0.0;
	zrot = 0.0;
	rotation_matrix = glm::mat4(1.0f);
	ortho_matrix = glm::mat4(1.0f);
	model_matrix = glm::mat4(1.0f);

	window_limits = glm::vec4(half_width, -half_width, half_height, -half_height);

	initShadersGL();
	initBuffersGL();

	eye = vcs[0];
	lookAtPoint = vcs[1];
	upVec = vcs[2];
	upVec = glm::normalize(upVec);

	// Frustum
	L = frustum[0];
	R = frustum[1];
	T = frustum[2];
	B = frustum[3];
  	N = frustum[4];
  	F = frustum[5];

  	calcStageTransformations();

  	addViewFrustum();
  	addEye();

  	updateCS(0);

  	// addModel("models/hut-new.raw", glm::vec3(0.3, 0.3, 0.3), glm::vec3(30.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/fan", glm::vec3(0.4, 0.4, 0.4), glm::vec3(0.0, 30.0, 60.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/chair", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 30.0, 30.0), glm::vec3(0.0, 0.0, 0.0));
	// addModel("models/goggles-new", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-90.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0));

  	// test();
}

void View::test() {
	model_matrix = vcs_to_ccs_matrix * wcs_to_vcs_matrix;
	for (glm::vec4 p : frustum_points) {
		std::cout << glm::to_string(p) << " - " << glm::to_string(model_matrix * p) << "\n";
	}
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
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uOrthoMatrix, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
	glUniform1i(uPerspectiveDivide, perspective_divide);
	glUniform1i(uToDCS, to_dcs);
	glUniform4fv(uWindowLimits, 1, glm::value_ptr(window_limits));
	glDrawArrays(GL_TRIANGLES, 0, points.size());

	glBindVertexArray(vao[1]);
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uOrthoMatrix, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
	glUniform1i(uPerspectiveDivide, perspective_divide);
	glUniform1i(uToDCS, to_dcs);
	glUniform4fv(uWindowLimits, 1, glm::value_ptr(window_limits));
	glDrawElements(GL_LINES, frustum_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(vao[2]);
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uOrthoMatrix, 1, GL_FALSE, glm::value_ptr(ortho_matrix));
	glUniform1i(uPerspectiveDivide, perspective_divide);
	glUniform1i(uToDCS, to_dcs);
	glUniform4fv(uWindowLimits, 1, glm::value_ptr(window_limits));
	glDrawArrays(GL_POINTS, 0, 1);
}

void View::updateRotationMatrix(GLuint axis, GLfloat angle) {
	// angle = deg_to_rad(-angle);
	// GLfloat sina = sin(angle), cosa = cos(angle);
	// glm::mat4 rot = glm::mat4(1.0f);
	// switch (axis) {
	// 	case 0:
	// 		// x-axis rotation
	// 		rot = glm::mat4(1.0, 0.0, 0.0, 0.0,
	// 	                   0.0, cosa, -sina, 0.0,
	// 	                   0.0, sina, cosa, 0.0,
	// 	                   0.0, 0.0, 0.0, 1.0);
	// 		break;

	// 	case 1:
	// 		rot = glm::mat4(cosa, 0.0, sina, 0.0,
 //                           0.0, 1.0, 0.0, 0.0,
 //                           -sina, 0.0, cosa, 0.0,
 //                           0.0, 0.0, 0.0, 1.0);
	// 		break;

	// 	case 2:
	// 		rot = glm::mat4(cosa, -sina, 0.0, 0.0,
 //                           sina, cosa, 0.0, 0.0,
 //                           0.0, 0.0, 1.0, 0.0,
 //                           0.0, 0.0, 0.0, 1.0);
	// 		break;
	// }

	// rotation_matrix = rot * rotation_matrix;

	angle = deg_to_rad(angle);
	switch (axis) {
		case 0:
			// x-axis rotation
			xrot += angle;
			break;
		case 1:
			yrot += angle;
			break;
		case 2:
			zrot += angle;
			break;
	}

	rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	updateCS(CS);
}

void View::updateCS(int val) {
	CS = val;
	switch (CS) {
		case 0:
			// WCS
			ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);
			model_matrix = rotation_matrix;
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 1:
			// VCS
			ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);
			model_matrix = rotation_matrix * wcs_to_vcs_matrix;
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 2:
			ortho_matrix = glm::ortho(-2.0, 2.0,
							  -2.0, 2.0,
							  -2.0, 2.0);
			model_matrix = rotation_matrix * vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 3:
			ortho_matrix = glm::ortho(-2.0, 2.0,
							  -2.0, 2.0,
							  -2.0, 2.0);
			model_matrix = rotation_matrix * vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 1;
			to_dcs = 0;
			break;
		case 4:
			ortho_matrix = glm::ortho(-half_width-50, half_width+50,
							  -half_height-50, half_height+50,
							  -half_depth-50, half_depth+50);
			model_matrix = rotation_matrix * vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 1;
			to_dcs = 1;
			break;
	}
}