#include "gl_framework.hpp"
#include "view.hpp"

extern View *v;

namespace toys {

  // !Initialize GL State
  void initGL(void) {
    // Set frame buffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Set depth buffer furthest depth
    glClearDepth(1.0);
    // Set depth test to less-than
    glDepthFunc(GL_LESS);
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    // Enable Gourard shading
    glShadeModel(GL_SMOOTH);

    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  // !GLFW Error Callback
  void error_callback(int error, const char* description) {
    std::cerr << description << std::endl;
  }

  // !GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // !Resize the view port to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }

  // !GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // !Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);

    switch (key) {
      // rotation
      case GLFW_KEY_UP:
        v->rotateCamera(1, 1);
        break;

      case GLFW_KEY_DOWN:
        v->rotateCamera(1, -1);
        break;

      case GLFW_KEY_LEFT:
        v->rotateCamera(0, 1);
        break;

      case GLFW_KEY_RIGHT:
        v->rotateCamera(0, -1);
        break;

      case GLFW_KEY_PAGE_UP:
        v->rotateCamera(2, 1);
        break;

      case GLFW_KEY_PAGE_DOWN:
        v->rotateCamera(2, -1);
        break;

      case GLFW_KEY_Z:
        v->zoom(5);
        break;

      case GLFW_KEY_X:
        v->zoom(-5);
        break;

      // model rotation
      case GLFW_KEY_W:
        v->rotateNode(1, 1);
        break;

      case GLFW_KEY_S:
        v->rotateNode(1, -1);
        break;

      case GLFW_KEY_A:
        v->rotateNode(0, 1);
        break;

      case GLFW_KEY_D:
        v->rotateNode(0, -1);
        break;

      case GLFW_KEY_Q:
        v->rotateNode(2, 1);
        break;

      case GLFW_KEY_E:
        v->rotateNode(2, -1);
        break;

      // select model
      case GLFW_KEY_B:
        // buzz
        v->selectModel(0);
        break;

      // select node
      case GLFW_KEY_0:
        v->selectNode(0);
        break;
      case GLFW_KEY_1:
        v->selectNode(1);
        break;
      case GLFW_KEY_2:
        v->selectNode(2);
        break;
      case GLFW_KEY_3:
        v->selectNode(3);
        break;
      case GLFW_KEY_4:
        v->selectNode(4);
        break;
      case GLFW_KEY_5:
        v->selectNode(5);
        break;
    }
  }
};  // namespace toys
