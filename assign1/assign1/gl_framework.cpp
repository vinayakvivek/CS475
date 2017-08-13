#include "gl_framework.hpp"
#include <vector>

extern std::vector<float> points;
extern int num_vertices;
extern int WIDTH, HEIGHT;

namespace CS475 {
	
	float curr_x, curr_y;
	
	// ! Initialize GL State
	void initGL(void)
	{
		// Set framebuffer clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Set depth buffer furthest depth
		glClearDepth(1.0);
		// Set depth test to less-than
		glDepthFunc(GL_LESS);
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);
		// Enable Gourard shading
		glShadeModel(GL_SMOOTH);
	}
	
	// !GLFW Error Callback
	void error_callback(int error, const char* description) {
		std::cerr << description << std::endl;
	}
	
	// !GLFW framebuffer resize callback
	void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		// !Resize the viewport to fit the window size - draw to entire window
		glViewport(0, 0, width, height);
		// printf("width: %d, height: %d\n", width, height);
		
		glfwGetWindowSize(window, &WIDTH, &HEIGHT);
	}
	
	// !GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// !Close the window if the ESC key was pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		else if (key == GLFW_KEY_A  && action == GLFW_PRESS) {
			points[0] = 100;
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		// std::cout << xpos << " " << ypos << std::endl;
		curr_x = xpos;
		curr_y = ypos;
	}
	
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << curr_x << " " << HEIGHT - curr_y << std::endl;
			points.push_back(curr_x);
			points.push_back(HEIGHT - curr_y);
			points.push_back(0.0);
			points.push_back(1.0);
			num_vertices++;
		}
	}
};
