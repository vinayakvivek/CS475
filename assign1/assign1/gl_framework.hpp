#ifndef gl_framework_hpp
#define gl_framework_hpp

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define BUFFER_OFFSET(offset) ((void *)(offset))

namespace CS475 {
	// ! Initialize GL State
	void initGL(void);
 
	// ! GLFW Error Callback
	void error_callback(int error, const char* description);
	// ! GLFW framebuffer resize callback
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// ! GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	
	void add_new_point(float x, float y);
	
	glm::vec4 get_random_color();
	
	void remove_last_point();
	
	void update_buffers();
};

#endif /* gl_framework_hpp */
