#ifndef gl_framework_hpp
#define gl_framework_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace modelling {
	
	enum state_enum:
	GLuint{
		s_model = 1,
		s_inspect = 2,
		s_start_polygon = 3,
		s_save = 4,
		s_load = 5,
	};
	
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
	
	void add_temp_point(GLfloat x, GLfloat y);
	
	void add_polygon();
	
	void remove_last_vertex();
	
	glm::vec4 get_random_color();
	
	void calc_centroid();
	
	void save_model();
	void load_model();
};

#endif /* gl_framework_hpp */
