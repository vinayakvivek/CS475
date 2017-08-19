#include "gl_framework.hpp"

extern std::vector<glm::vec4> temp_points;
extern std::vector<glm::vec4> temp_colors;
extern std::vector<glm::vec4> points;
extern std::vector<glm::vec4> colors;
extern GLfloat half_width, half_height, half_depth;
extern modelling::state_enum state;

extern GLfloat theta;
extern GLfloat xrot, yrot, zrot;
extern GLfloat xpos, ypos, zpos;

extern glm::mat4 translate_matrix;
extern glm::mat4 rotation_matrix;

extern GLboolean show_planes;

namespace modelling {
	
	GLfloat curr_x, curr_y;
	
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
	void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
		// !Resize the viewport to fit the window size - draw to entire window
		glViewport(0, 0, w, h);
	}
	
	// !GLFW keyboard callback
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		// !Close the window if the ESC key was pressed
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		} else if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			switch (state) {
				case s_model:
					state = s_start_polygon;
					break;
				case s_start_polygon:
					state = s_model;
					add_polygon();
					break;
				default:
					break;
			}
		} else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			// toggle planes
			show_planes = !show_planes;
		} else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				yrot -= theta;
			}
		} else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				yrot += theta;
			}
		} else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				xrot -= theta;
			}
		} else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				xrot += theta;
			}
		} else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				zrot -= theta;
			}
		} else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) {
			if (state != s_start_polygon) {
				zrot += theta;
			}
		}

	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		curr_x = xpos;
		curr_y = ypos;
	}
	
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			if (state == s_start_polygon)
				add_temp_point(curr_x - half_width, half_height - curr_y);
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			remove_last_vertex();
		}
	}
	
	void add_temp_point(GLfloat x, GLfloat y) {
		std::cout << x << " " << y << "\n";
		glm::vec4 p = glm::transpose(rotation_matrix) * glm::vec4(x, y, 0.0f, 1.0f);
		temp_points.push_back(p);
		temp_colors.push_back(get_random_color());
	}
	
	void add_polygon() {
		int n = temp_points.size();
		
		if (n < 3)
			return;
		
		glm::vec4 centre;
		for (glm::vec4 point : temp_points) {
			centre += point;
		}
		centre /= n;
		glm::vec4 centre_color = get_random_color();
		
		for (int i = 0; i < n - 1; ++i) {
			points.push_back(centre);
			points.push_back(temp_points[i]);
			points.push_back(temp_points[i + 1]);
			
			colors.push_back(centre_color);
			colors.push_back(temp_colors[i]);
			colors.push_back(temp_colors[i + 1]);
		}
		
		points.push_back(centre);
		points.push_back(temp_points[n - 1]);
		points.push_back(temp_points[0]);
		
		colors.push_back(centre_color);
		colors.push_back(temp_colors[n - 1]);
		colors.push_back(temp_colors[0]);
		
		temp_points.clear();
		temp_colors.clear();
	}
	
	void remove_last_vertex() {
		int n = temp_points.size();
		if (n > 0) {
			temp_points.pop_back();
			temp_colors.pop_back();
		} else if (points.size() >= 3) {
			points.pop_back();
			points.pop_back();
			points.pop_back();
			
			colors.pop_back();
			colors.pop_back();
			colors.pop_back();
		}
	}
	
	glm::vec4 get_random_color() {
		float t = (float)rand()/(float)RAND_MAX;
		return glm::vec4(
						 9*(1-t)*t*t*t,
						 15*(1-t)*(1-t)*t*t,
						 8.5*(1-t)*(1-t)*(1-t)*t,
						 1.0
						 );
	}
};

















