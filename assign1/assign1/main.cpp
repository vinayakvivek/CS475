//
//  main.cpp
//  Xcode-OpenGL-template
//
//  Created by vinayak vivek on 8/7/17.
//  Copyright © 2017 vinayak. All rights reserved.
//

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#define BUFFER_OFFSET(offset) ((void *)(offset))

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int WIDTH = 800, HEIGHT = 600;

int num_vertices = 6;

//float points[] = {
//	10.0f,  500.0f,  0.0f, 1.0f,
//	800.0f, 100.0f,  0.0f, 1.0f,
//	0.0f, 0.0f,  0.0f, 1.0f,
//};
//
//float points2[] = {
//	10.0f,  500.0f,  0.0f, 1.0f,
//	400.0f, 100.0f,  0.0f, 1.0f,
//	0.0f, 0.0f,  0.0f, 1.0f,
//};

std::vector<float> points({
	10.0f,  200.0f,  0.0f, 1.0f,
	400.0f, 100.0f,  0.0f, 1.0f,
	10.0f, 10.0f,  0.0f, 1.0f,
	500.0f,  200.0f,  0.0f, 1.0f,
	400.0f, 100.0f,  0.0f, 1.0f,
	400.0f, 10.0f,  0.0f, 1.0f,
});

GLuint shaderProgram;
GLuint vbo, vao;

glm::mat4 translate_matrix;
glm::mat4 rotation_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;

void initBuffersGL(void)
{
	//Ask GL for a Vertex Attribute Object (vao)
	glGenVertexArrays (1, &vao);
	//Set it as the current array to be used by binding it
	glBindVertexArray (vao);
	
	//Ask GL for a Vertex Buffer Object (vbo)
	glGenBuffers (1, &vbo);
	//Set it as the current buffer to be used by binding it
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	//Copy the points into the current buffer
	glBufferData (GL_ARRAY_BUFFER, num_vertices * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	// glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	// glBufferSubData(GL_ARRAY_BUFFER, sizeof(v_positions), sizeof(v_colors), v_colors);
	
	// Load shaders and use the resulting shader program
	std::string vertex_shader_file("shaders/vert.shader");
	std::string fragment_shader_file("shaders/frag.shader");
	
	std::vector<GLuint> shaderList;
	shaderList.push_back(CS475::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(CS475::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
	
	shaderProgram = CS475::createProgramGL(shaderList);
	glUseProgram( shaderProgram );
	
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
//	GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" );
//	glEnableVertexAttribArray( vColor );
//	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(v_positions)));
	
	uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");
}

void renderGL(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBufferData (GL_ARRAY_BUFFER, num_vertices * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices * 4 * sizeof(float), &points[0]);
	
	translate_matrix =  glm::translate(glm::mat4(1.f), glm::vec3(-1.0 * WIDTH/2, -1.0 * HEIGHT/2, 0.f));
	ortho_matrix = glm::ortho(-1.0 * WIDTH / 2, 1.0 * WIDTH/2, -1.0 * HEIGHT/2, 1.0 * HEIGHT/2, -1.0, 1.0);
	
	modelview_matrix = ortho_matrix * translate_matrix;
	
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	
	// Draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
}

int main(int argc, char** argv) {
	//! The pointer to the GLFW window
	GLFWwindow* window;
	
	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(CS475::error_callback);
	
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
	
	// glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	// ! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "CS475 assignment 1", NULL, NULL);
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
	glfwSetKeyCallback(window, CS475::key_callback);
	// Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, CS475::framebuffer_size_callback);
	
	glfwSetCursorPosCallback(window, CS475::cursor_position_callback);
	glfwSetMouseButtonCallback(window, CS475::mouse_button_callback);
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	
	// Initialize GL state
	CS475::initGL();
	initBuffersGL();
	
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
