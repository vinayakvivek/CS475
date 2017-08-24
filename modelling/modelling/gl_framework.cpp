#include "gl_framework.hpp"

extern std::vector<glm::vec4> temp_points;
extern std::vector<glm::vec4> temp_colors;
extern std::vector<glm::vec4> points;
extern std::vector<glm::vec4> colors;
extern GLfloat half_width, half_height, half_depth;
extern modelling::state_enum state;

extern GLfloat theta, delta;
extern GLfloat xrot, yrot, zrot;
extern GLfloat xpos, ypos, zpos;

extern glm::mat4 translate_matrix;
extern glm::mat4 rotation_matrix;
extern glm::mat4 model_matrix;

extern glm::vec4 centroid;
extern GLboolean show_planes;
extern GLboolean show_frontal_plane;

namespace modelling {
	
	GLfloat curr_x, curr_y;
	glm::vec4 diff;
	
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
		
		diff = glm::vec4(0.0, 0.0, 0.0, 1.0);
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
		} else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
			state = s_model;
			xpos = ypos = zpos = 0.0;
		} else if (key == GLFW_KEY_I && action == GLFW_PRESS) {
			state = s_inspect;
			calc_centroid();
			xpos = ypos = zpos = 0.0;
		} else if (key == GLFW_KEY_K && action == GLFW_PRESS) {
			state_enum prev_state = state;
			state = s_save;
			save_model();
			state = prev_state;
		} else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			state = s_load;
			load_model();
			state = s_inspect;
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
		} else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
			// toggle planes
			show_planes = !show_planes;
		} else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
			show_frontal_plane = !show_frontal_plane;
		} else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			// move centroid to origin
			if (state == s_model)
				calc_centroid();
			xpos = ypos = zpos = 0.0;
		} else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				yrot -= theta;
			//}
		} else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				yrot += theta;
			//}
		} else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				xrot -= theta;
			//}
		} else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				xrot += theta;
			//}
		} else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				zrot -= theta;
			//}
		} else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				zrot += theta;
			//}
		} else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			ypos += delta;
		} else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			ypos -= delta;
		} else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			xpos += delta;
		} else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			xpos -= delta;
		} else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			zpos += delta;
		} else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			zpos -= delta;
		} else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				zrot = 0; yrot = 0;
				xrot = 3.1456 / 2;
			//}
		} else if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				xrot = 0; zrot = 0;
				yrot = 0;
			//}
		} else if (key == GLFW_KEY_U && action == GLFW_PRESS) {
			//if (state != s_start_polygon) {
				xrot = 0; zrot = 0;
				yrot = 3.1456 / 2;
			//}
		}

	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
		curr_x = xpos;
		curr_y = ypos;
		if (state == s_start_polygon) {
			update_temp_point(curr_x - half_width, half_height - curr_y);
		}
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
		// std::cout << x << " " << y << "\n";
		glm::vec4 p = glm::inverse(model_matrix) * glm::vec4(x, y, 0.0f, 1.0f);
		temp_points.push_back(p);
		temp_colors.push_back(get_random_color());
	}
	
	void update_temp_point(GLfloat x, GLfloat y) {
		if (temp_points.size() < 1)
			return;
		
		glm::vec4 p = glm::inverse(model_matrix) * glm::vec4(x, y, 0.0f, 1.0f);
		temp_points[temp_points.size() - 1] = p;
	}
	
	void add_polygon() {
		int n = temp_points.size();
		
		if (n < 3)
			return;
		
		temp_points.pop_back();
		
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
		
		// calc_centroid();
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
		
		// calc_centroid();
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
	
	bool comp_unique(const glm::vec4 &a, const glm::vec4 &b) {
		float eps = 0.00001;
		return (abs(a.x - b.x) < eps &&
				abs(a.y - b.y) < eps &&
				abs(a.z - b.z) < eps &&
				abs(a.w - b.w) < eps);
	}
	
	bool comp_sort(const glm::vec4 &a, const glm::vec4 &b) {
		if (a.x < b.x)
			return true;
		else if (a.y < b.y)
			return true;
		else if (a.z < b.z)
			return true;
		else
			return false;
	}
	
	void calc_centroid() {
		glm::vec4 p = glm::vec4(0.0, 0.0, 0.0, 1.0);
		std::vector<glm::vec4> t_points = points;
		
//		std::sort(t_points.begin(), t_points.end(), comp_sort);
//		auto last = std::unique(t_points.begin(), t_points.end(), comp_unique);
//		t_points.erase(last, t_points.end());
		
		std::cout << "no. of points: " << t_points.size() << std::endl;
		
		for (glm::vec4 point : t_points) {
			p += point;
		}
		p /= points.size();
				
		centroid = p;
		
		std::cout << p.x << " " << p.y << " " << p.z << "\n";
	}
	
	void save_model() {
		std::string file_name;
		std::cin >> file_name;
		
		std::ofstream model_file;
		model_file.open(file_name, std::ios::out);
		
		int n = points.size();
		for (int i = 0; i < n; ++i) {
			model_file << points[i].x << " " << points[i].y << " " << points[i].z << " ";
			model_file << colors[i].x << " " << colors[i].y << " " << colors[i].z << "\n";
		}
		
		model_file.close();
	}
	
	void load_model() {
		std::string file_name;
		std::cin >> file_name;
		
		std::ifstream model_file;
		model_file.open(file_name, std::ios::in);
		
		points.clear();
		colors.clear();
		
		GLfloat x, y, z, r, g, b;
		while (model_file >> x >> y >> z >> r >> g >> b) {
			points.push_back(glm::vec4(x, y, z, 1.0));
			colors.push_back(glm::vec4(r, g, b, 1.0));
		}
		
		model_file.close();
	}
};

















