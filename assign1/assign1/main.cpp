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

float WIDTH = 800, HEIGHT = 600, DEPTH = 600;

//int num_vertices = 7;
//int buffer_size;
//int index_buffer_size;
//
//std::vector<GLfloat> points({
//	-100.0f,  100.0f,  100.0f, 1.0f,
//	-100.0f, -100.0f,  100.0f, 1.0f,
//	100.0f, 100.0f,  100.0f, 1.0f,
//	100.0f, -100.0f,  100.0f, 1.0f,
//	
//	-100.0f, -100.0f, -100.0f, 1.0f,
//	-100.0f, 100.0f, -100.0f, 1.0f,
//	
//	100.0, 100.0f, -100.0f, 1.0f,
//});
//
//std::vector<GLuint> indices({
//	0, 1, 2,
//	2, 1, 3,
//	
//	0, 4, 1,
//	0, 5, 4,
//	
//	0, 2, 5,
//	2, 6, 5,
//});
//
//std::vector<GLfloat> colors({
//	0.0, 0.0, 0.0, 1.0,
//	1.0, 0.0, 0.0, 1.0,
//	1.0, 1.0, 0.0, 1.0,
//	1.0, 0.5, 0.5, 1.0,
//	
//	1.0, 0.25, 0.75, 1.0,
//	0.30, 0.8, 0.5, 1.0,
//	
//	0.50, 0.2, 0.5, 1.0,
//});

int num_vertices = 0;
unsigned long buffer_size, index_buffer_size;

std::vector<GLfloat> points;
std::vector<GLuint> indices;
std::vector<GLfloat> colors;

GLuint shaderProgram;
GLuint vbo[3], vao[3];
GLuint eab;
GLint vColor, vPosition;

// Rotation Parameters
GLfloat xrot = 0.0, yrot = 0.0, zrot = 0.0;
GLfloat xpos = 0.0, ypos = 0.0, zpos = 0.0;

float drawing_plane[] = {
	-WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	
	-WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	-WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
};

float drawing_plane_colors[] = {
	0.5, 0.2, 0.3, 0.5,
	0.5, 0.2, 0.3, 0.5,
	0.5, 0.2, 0.3, 0.5,
	0.5, 0.2, 0.3, 0.5,
	0.5, 0.2, 0.3, 0.5,
	0.5, 0.2, 0.3, 0.5,
};

float planes[] = {
	-WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	
	-WIDTH/2, HEIGHT/2, 0.0f, 1.0f,
	-WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
	WIDTH/2, -HEIGHT/2, 0.0f, 1.0f,
	
	-WIDTH/2, 0.0f, -DEPTH/2, 1.0f,
	-WIDTH/2, 0.0f, DEPTH/2, 1.0f,
	WIDTH/2, 0.0f, -DEPTH/2, 1.0f,
	
	-WIDTH/2, 0.0f, DEPTH/2, 1.0f,
	WIDTH/2, 0.0f, DEPTH/2, 1.0f,
	WIDTH/2, 0.0f, -DEPTH/2, 1.0f,
};
float plane_colors[] = {
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5,
};

glm::mat4 translate_matrix;
glm::mat4 rotation_matrix;
glm::mat4 ortho_matrix;
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;

void loadShaderProgram() {
	// Load shaders and use the resulting shader program
	std::string vertex_shader_file("shaders/vert.shader");
	std::string fragment_shader_file("shaders/frag.shader");
	
	std::vector<GLuint> shaderList;
	shaderList.push_back(CS475::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
	shaderList.push_back(CS475::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));
	
	shaderProgram = CS475::createProgramGL(shaderList);
	glUseProgram(shaderProgram);
}

void initBuffersGL(void) {
	
	buffer_size = points.size() * sizeof(GLfloat);
	index_buffer_size = indices.size() * sizeof(GLuint);
	
	loadShaderProgram();
	
	glGenVertexArrays(3, vao);
	glGenBuffers(3, vbo);
	
	
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
	
	// set up vertex arrays
	vPosition = glGetAttribLocation(shaderProgram, "vPosition" );
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	// Get color attribure
	vColor = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	// glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_size));
	
	uModelViewMatrix = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
	
	
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// drawing plane
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof (drawing_plane) + sizeof(drawing_plane_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(drawing_plane), drawing_plane);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(drawing_plane), sizeof(drawing_plane_colors), drawing_plane_colors);
	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(drawing_plane)));
	
	// planes
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof (planes) + sizeof(plane_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(planes), planes);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(planes), sizeof(plane_colors), plane_colors);
	
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(planes)));
	
	// main points
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &eab);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, &indices[0], GL_DYNAMIC_DRAW);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void renderGL(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f,0.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f,1.0f,0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f,0.0f,1.0f));
	translate_matrix =  glm::translate(glm::mat4(1.f), glm::vec3(xpos, ypos, zpos));
	ortho_matrix = glm::ortho(-1.0 * WIDTH / 2, 1.0 * WIDTH/2,
							  -1.0 * HEIGHT/2, 1.0 * HEIGHT/2,
							  -1.0 * DEPTH/2, 1.0 * DEPTH/2);
	
	// draw points
	modelview_matrix = ortho_matrix * translate_matrix * rotation_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
	
	// draw drawing plane
	modelview_matrix = ortho_matrix;
	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	// draw planes
//	modelview_matrix = ortho_matrix * rotation_matrix;
//	glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelview_matrix));
//	glBindVertexArray(vao[2]);
//	glDrawArrays(GL_TRIANGLES, 0, 12);
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
	window = glfwCreateWindow((int)WIDTH, (int)HEIGHT, "CS475 assignment 1", NULL, NULL);
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
