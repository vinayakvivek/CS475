#include "model.hpp"


void initShadersGL(void) {
	std::string vertex_shader_file("shaders/vert.shader");
	std::string fragment_shader_file("shaders/frag.shader");
	
	std::vector<GLuint> shaderList;
	shaderList.push_back(modelling::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(modelling::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
	
	shaderProgram = modelling::createProgramGL(shaderList);
	glUseProgram(shaderProgram);
}

void update_buffer_lengths() {
	points_buffer_length = points.size() * 4 * sizeof(GLfloat);
	temp_points_buffer_length = temp_points.size() * 4 * sizeof(GLfloat);
	colors_buffer_length = colors.size() * 4 * sizeof(GLfloat);
	temp_colors_buffer_length = temp_colors.size() * 4 * sizeof(GLfloat);
}

void initBufferGL(void) {
	
	initShadersGL();
	vPosition = glGetAttribLocation(shaderProgram, "vPosition" );
	uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
	
	glGenBuffers(num_vbo, vbo);
	glGenVertexArrays(num_vao, vao);
	
	
	// main model
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindVertexArray(vao[0]);
	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_length));

	
	// temp model polygon
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBindVertexArray(vao[1]);
	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	
	glEnable(GL_PROGRAM_POINT_SIZE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void renderGL(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	
	ortho_matrix = glm::ortho(-half_width, half_width,
							  -half_height, half_height,
							  -half_depth, half_depth);
	
	modelview_matrix = ortho_matrix * rotation_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
	update_buffer_lengths();
	
	// main figure
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindVertexArray(vao[0]);
	glBufferData(GL_ARRAY_BUFFER, points_buffer_length + colors_buffer_length, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, points_buffer_length, &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, points_buffer_length, colors_buffer_length, &colors[0]);
	
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_length));
	glDrawArrays(GL_TRIANGLES, 0, points.size());
	
	// temp polygon points
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBindVertexArray(vao[1]);
	glBufferData(GL_ARRAY_BUFFER, temp_points_buffer_length + temp_colors_buffer_length, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, temp_points_buffer_length, &temp_points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, temp_points_buffer_length, temp_colors_buffer_length, &temp_colors[0]);
	
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(temp_points_buffer_length));
	glDrawArrays(GL_POINTS, 0, temp_points.size());
}

int main(int argc, char** argv) {
	//! The pointer to the GLFW window
	GLFWwindow* window;
	
	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(modelling::error_callback);
	
	//! Initialize GLFW
	if (!glfwInit())
		return -1;
	
	// We want OpenGL 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// This is for MacOSX - can be omitted otherwise
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// We don't want the old OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// ! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(half_width * 2, half_height * 2, "CS475 modelling", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	
	// ! Make the window's context current
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	// Turn this on to get Shader based OpenGL
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		// Problem: glewInit failed, something is seriously wrong.
		std::cerr<<"GLEW Init Failed : %s"<<std::endl;
	}
	
	// Print and see what context got enabled
	std::cout << "Vendor: " << glGetString (GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString (GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString (GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;
	
	// Keyboard Callback
	glfwSetKeyCallback(window, modelling::key_callback);
	// Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, modelling::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, modelling::cursor_position_callback);
	glfwSetMouseButtonCallback(window, modelling::mouse_button_callback);
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	// Initialize GL state
	modelling::initGL();
	
	initBufferGL();
	
	// Loop until the user closes the window
	while (glfwWindowShouldClose(window) == 0) {
	
		// Render here
		renderGL();
		
		// Swap front and back buffers
		glfwSwapBuffers(window);
		
		// Poll for and process events
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
