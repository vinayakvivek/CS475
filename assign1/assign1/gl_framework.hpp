#ifndef gl_framework_hpp
#define gl_framework_hpp

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
};

#endif /* gl_framework_hpp */
