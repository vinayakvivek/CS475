#include "gl_framework.hpp"
#include "view.hpp"

extern View *v;

namespace viewing
{
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
	}

	// !GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// !Close the window if the ESC key was pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;

			// rotation
			case GLFW_KEY_UP:
				v->updateRotationMatrix(1, 1);
				break;

			case GLFW_KEY_DOWN:
				v->updateRotationMatrix(1, -1);
				break;

			case GLFW_KEY_LEFT:
				v->updateRotationMatrix(0, 1);
				break;

			case GLFW_KEY_RIGHT:
				v->updateRotationMatrix(0, -1);
				break;

			case GLFW_KEY_PAGE_UP:
				v->updateRotationMatrix(2, 1);
				break;

			case GLFW_KEY_PAGE_DOWN:
				v->updateRotationMatrix(2, -1);
				break;

			// coordinate system change
			case GLFW_KEY_0:
				// WCS
				v->updateCS(0);
				break;

			case GLFW_KEY_1:
				// VCS
				v->updateCS(1);
				break;

			case GLFW_KEY_2:
				// CCS
				v->updateCS(2);
				break;

		}
	}
};
