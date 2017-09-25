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
	uViewMatrix = glGetUniformLocation(shaderProgram, "uViewMatrix");
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
	view_matrix = glm::mat4(1.0f);
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

  	// calculates and sets WCS <-> VCS <-> CCS transformations
  	calcStageTransformations();

  	addViewFrustum();
  	addEye();

  	// default coordinate system : WCS
  	updateCS(0);
  	clipped = false;
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

	int old_num_points = points.size();

	points.insert(points.end(), t_points->begin(), t_points->end());
	colors.insert(colors.end(), m->getColors().begin(), m->getColors().end());

	populateClippedPoints(old_num_points);
	unClip();
}

void View::renderGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao[0]);
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniform1i(uPerspectiveDivide, perspective_divide);
	glUniform1i(uToDCS, to_dcs);
	glUniform4fv(uWindowLimits, 1, glm::value_ptr(window_limits));
	glDrawArrays(GL_TRIANGLES, 0, num_points);

	glBindVertexArray(vao[1]);
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniform1i(uPerspectiveDivide, perspective_divide);
	glUniform1i(uToDCS, to_dcs);
	glUniform4fv(uWindowLimits, 1, glm::value_ptr(window_limits));
	glDrawElements(GL_LINES, frustum_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(vao[2]);
	glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(uViewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
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
	if (val != CS) {
		xrot = 0.0;
		yrot = (val != 0 && val != 1) ? PI : 0.0;
		zrot = 0.0;
		CS = val;
		rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
		rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
		rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	}

	switch (CS) {
		case 0:
			// WCS
			ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);
			view_matrix = ortho_matrix * rotation_matrix;
			model_matrix = glm::mat4(1.0f);
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 1:
			// VCS
			ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);
			view_matrix = ortho_matrix * rotation_matrix;
			model_matrix = wcs_to_vcs_matrix;
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 2:
			ortho_matrix = glm::ortho(-2.0, 2.0,
							  -2.0, 2.0,
							  -2.0, 2.0);
			view_matrix = ortho_matrix * rotation_matrix;
			model_matrix = vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 0;
			to_dcs = 0;
			break;
		case 3:
			ortho_matrix = glm::ortho(-2.0, 2.0,
							  -2.0, 2.0,
							  -2.0, 2.0);
			view_matrix = ortho_matrix * rotation_matrix;
			model_matrix = vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 1;
			to_dcs = 0;
			break;
		case 4:
			ortho_matrix = glm::ortho(-half_width-50, half_width+50,
							  -half_height-50, half_height+50,
							  -half_depth-50, half_depth+50);
			view_matrix = ortho_matrix * rotation_matrix;
			model_matrix = vcs_to_ccs_matrix * wcs_to_vcs_matrix;
			perspective_divide = 1;
			to_dcs = 1;
			break;
	}
}

// planes
glm::vec4 planes[] = {
	glm::vec4(1, 0, 0, 1),  // X1
	glm::vec4(-1, 0, 0, 1),  // X2
	glm::vec4(0, 1, 0, 1),   // Y1
	glm::vec4(0, -1, 0, 1),  // Y2
	glm::vec4(0, 0, 1, 1),  // Z1
	glm::vec4(0, 0, -1, 1)   // Z2
};

GLuint outCode(const glm::vec4 &p) {
	GLuint out_code = 0;
    for (int i = 0; i < 6; ++i) {
        if (glm::dot(p, planes[i]) < 0) out_code |= (1 << i);
    }
    return out_code;
}

void clipWithPlane(int plane_id, std::vector<glm::vec4> &poly, std::vector<glm::vec4> &colors) {
	glm::vec4 plane = planes[plane_id];

	std::vector<glm::vec4> new_poly;
	std::vector<glm::vec4> new_colors;
	int n = poly.size();
	glm::vec4 p1, p2, c1, c2;

	for (int i = 0; i < n; ++i) {
		p1 = poly[i]; p2 = poly[(i+1)%n];
		c1 = colors[i]; c2 = colors[(i+1)%n];

		GLfloat a1, a2, alpha;
		a1 = dot(p1, plane);
		a2 = dot(p2, plane);

		if (a1 == a2)
			return;

		alpha = a1 / (a1 - a2);
		glm::vec4 pi = p1 * (1 - alpha) + p2 * alpha;

		if (a1 > 0 && a2 > 0) {
			new_poly.push_back(p2);
			new_colors.push_back(c2);
		} else if (a1 > 0 && a2 < 0) {
			new_poly.push_back(pi);
			new_colors.push_back(c2);
		} else if (a1 < 0 && a2 > 0) {
			new_poly.push_back(pi);
			new_poly.push_back(p2);
			new_colors.push_back(c1);
			new_colors.push_back(c2);
		}
	}

	poly = new_poly;
	colors = new_colors;
}

void View::populateClippedPoints(int offset) {
	glm::mat4 matrix = vcs_to_ccs_matrix * wcs_to_vcs_matrix;
	glm::mat4 inv = glm::inverse(matrix);

	int num_traingles =  (points.size() - offset) / 3;

	for (int k = 0; k < num_traingles; ++k) {

		int p1 = offset + 3*k;
		int p2 = p1 + 1;
		int p3 = p1 + 2;

		std::vector<glm::vec4> new_poly({matrix * points[p1], matrix * points[p2], matrix * points[p3]});
		std::vector<glm::vec4> new_colors({colors[p1], colors[p2], colors[p3]});

		for (int i = 0; i < 6; ++i) {
			clipWithPlane(i, new_poly, new_colors);
		}

		for (int i = 0; i < new_poly.size(); ++i) {
			new_poly[i] = inv * new_poly[i];
		}

		std::vector<glm::vec4> new_points;
		std::vector<glm::vec4> colors;
		int n = new_poly.size();
		for (int i = 0; i < n - 2; ++i) {
			new_points.push_back(new_poly[0]);
			new_points.push_back(new_poly[(i+1)%n]);
			new_points.push_back(new_poly[(i+2)%n]);

			colors.push_back(new_colors[0]);
			colors.push_back(new_colors[(i+1)%n]);
			colors.push_back(new_colors[(i+2)%n]);
		}

		clipped_points.insert(clipped_points.end(), new_points.begin(), new_points.end());
		clipped_colors.insert(clipped_colors.end(), colors.begin(), colors.end());
	}
}

void View::clip() {
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	GLuint buffer_length = clipped_points.size() * 4 * sizeof(GLfloat);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_length));

	glBufferData(GL_ARRAY_BUFFER, buffer_length + buffer_length, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_length, &clipped_points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, buffer_length, buffer_length, &clipped_colors[0]);

	clipped = true;
	num_points = clipped_points.size();
}

void View::unClip() {
	GLuint points_buffer_length = points.size() * 4 * sizeof(GLfloat);
	GLuint colors_buffer_length = colors.size() * 4 * sizeof(GLfloat);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_length));

	glBufferData(GL_ARRAY_BUFFER, points_buffer_length + colors_buffer_length, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, points_buffer_length, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, points_buffer_length, colors_buffer_length, &colors[0]);

	clipped = false;
	num_points = points.size();
}