#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "view.hpp"

#include <sstream>
#include <string>

#define NMODEL 1

GLfloat half_width = 400, half_height = 400, half_depth = 400;
View *v;

struct ModelData {
	std::string name;
	glm::vec3 scale, rot, trans;

	void print() {
		std::cout << name << " : ";
		std::cout << glm::to_string(scale) << " ";
		std::cout << glm::to_string(rot) << " ";
		std::cout << glm::to_string(trans) << "\n";
	}
};

ModelData *readModelData(std::ifstream &scene_file) {
	ModelData *data = new ModelData;
	std::string line;
	int lines_read = 0;
	while (std::getline(scene_file, line)) {
		if (line[0] != '#' && !line.empty()) {
			if (lines_read == 0) {
				data->name = line;
			} else {
				std::istringstream iss(line);
				float a, b, c;
				if (!(iss >> a >> b >> c)) {
					std::cout << "error reading!\n";
					break;
				}

				if (lines_read == 1)
					data->scale = glm::vec3(a, b, c);
				else if (lines_read == 2)
					data->rot = glm::vec3(a, b, c);
				else if (lines_read == 3)
					data->trans = glm::vec3(a, b, c);
			}
			lines_read++;
			if (lines_read == 4)
				break;
		}
	}
	return data;
}

void initView() {
	std::string file_name = "myscene.scn";
	std::ifstream scene_file;
	scene_file.open(file_name, std::ios::in);

	if (!scene_file.is_open()) {
		std::cout << "could not load scene file :( \n";
		return;
	}

	ModelData *models[NMODEL];
	for (int i = 0; i < NMODEL; ++i)
		models[i] = readModelData(scene_file);

	// VCS setup
	glm::vec3 eye;
	glm::vec3 lookAtPoint;
	glm::vec3 upVec;

	// Frustum
	GLfloat L, R, T, B;
  	GLfloat N, F;

  	std::string line;
	int lines_read = 0;
	while (std::getline(scene_file, line) && lines_read <= 5) {
		if (line[0] != '#' && !line.empty()) {
			float a, b, c;
			std::istringstream iss(line);
			switch (lines_read) {
				case 0:
					iss >> a >> b >> c;
					eye = glm::vec3(a, b, c);
					break;
				case 1:
					iss >> a >> b >> c;
					lookAtPoint = glm::vec3(a, b, c);
					break;
				case 2:
					iss >> a >> b >> c;
					upVec = glm::vec3(a, b, c);
					break;
				case 3:
					iss >> L >> R >> T >> B;
					break;
				case 4:
					iss >> N >> F;
					break;
			}
			lines_read++;
		}
	}

	glm::vec3 vcs[] = {eye, lookAtPoint, upVec};
	GLfloat frustum[] = {L, R, T, B, N, F};
	v = new View(half_width, half_height, half_depth, vcs, frustum);

	for (int i = 0; i < NMODEL; ++i) {
		v->addModel(models[i]->name, models[i]->scale, models[i]->rot, models[i]->trans);
	}

	scene_file.close();
}

int main(int argc, char** argv) {
	//! The pointer to the GLFW window
	GLFWwindow* window;

	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(viewing::error_callback);

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

	// ! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(half_width * 2, half_height * 2, "CS475 Assignment 2 : Viewing", NULL, NULL);
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
	glfwSetKeyCallback(window, viewing::key_callback);
	// Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, viewing::framebuffer_size_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Initialize GL state
	viewing::initGL();
	initView();

	// Loop until the user closes the window
	while (glfwWindowShouldClose(window) == 0) {

		// Render here
		v->renderGL();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
