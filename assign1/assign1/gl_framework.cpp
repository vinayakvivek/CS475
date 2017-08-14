#include "gl_framework.hpp"
#include <vector>

extern std::vector<GLfloat> points;
extern std::vector<GLuint> indices;
extern std::vector<GLfloat> colors;
extern int num_vertices;
extern int WIDTH, HEIGHT;

extern GLuint shaderProgram;
extern GLuint vbo, vao;
extern GLint vColor, vPosition;

extern unsigned long buffer_size, index_buffer_size;

extern GLfloat xrot, yrot, zrot;
extern GLfloat xpos, ypos, zpos;

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
		else if (key == GLFW_KEY_A  && action == GLFW_PRESS)
			yrot -= 0.1;
		else if (key == GLFW_KEY_D  && action == GLFW_PRESS)
			yrot += 0.1;
		else if (key == GLFW_KEY_W  && action == GLFW_PRESS)
			xrot -= 0.1;
		else if (key == GLFW_KEY_S  && action == GLFW_PRESS)
			xrot += 0.1;
		else if (key == GLFW_KEY_Q  && action == GLFW_PRESS)
			zrot -= 0.1;
		else if (key == GLFW_KEY_E  && action == GLFW_PRESS)
			zrot += 0.1;
		else if (key == GLFW_KEY_Z  && action == GLFW_PRESS)
			zpos -= 50.0;
		else if (key == GLFW_KEY_X  && action == GLFW_PRESS)
			zpos += 50.0;
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		// std::cout << xpos << " " << ypos << std::endl;
		curr_x = xpos;
		curr_y = ypos;
	}
	
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			add_new_point(curr_x - WIDTH/2, HEIGHT/2 - curr_y);
		} else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			remove_last_point();
		}
	}
	
	void add_new_point(float x, float y) {
//		xrot = yrot = zrot = 0;
		glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), -xrot, glm::vec3(1.0f,0.0f,0.0f));
		rotation_matrix = glm::rotate(rotation_matrix, -yrot, glm::vec3(0.0f,1.0f,0.0f));
		rotation_matrix = glm::rotate(rotation_matrix, -zrot, glm::vec3(0.0f,0.0f,1.0f));
		
		glm::vec4 new_point(x, y, 0.0, 1.0);
		new_point = rotation_matrix * new_point;
		
		points.push_back(new_point.x);
		points.push_back(new_point.y);
		points.push_back(new_point.z);
		points.push_back(1.0);
		
		num_vertices++;
		
		glm::vec4 new_color = get_random_color();
		colors.push_back(new_color.x);
		colors.push_back(new_color.y);
		colors.push_back(new_color.z);
		colors.push_back(new_color.w);
		
		size_t n = indices.size();
		if (n > 2) {
			indices.push_back(indices[n - 2]);
			indices.push_back(indices[n - 1]);
			indices.push_back(num_vertices - 1);
		} else {
			if (num_vertices > 2) {
				indices.push_back(0);
				indices.push_back(1);
				indices.push_back(2);
			}
		}
		
		update_buffers();
	}
	
	void remove_last_point() {
		points.pop_back();
		points.pop_back();
		points.pop_back();
		points.pop_back();
		
		colors.pop_back();
		colors.pop_back();
		colors.pop_back();
		colors.pop_back();
		
		num_vertices--;
		
		indices.pop_back();
		indices.pop_back();
		indices.pop_back();
		
		update_buffers();
	}
	
	void update_buffers() {
		buffer_size = points.size() * sizeof(GLfloat);
		index_buffer_size = indices.size() * sizeof(GLuint);
		
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_size));
		
		glBufferData(GL_ARRAY_BUFFER, buffer_size * 2, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size, &points[0]);
		glBufferSubData(GL_ARRAY_BUFFER, buffer_size, buffer_size, &colors[0]);
		
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_size, &indices[0], GL_DYNAMIC_DRAW);
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
