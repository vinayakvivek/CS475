#include "view.hpp"

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth) {
  half_width = h_width;
  half_height = h_height;
  half_depth = h_depth;

  initShadersGL();

  ortho_matrix = glm::ortho(-half_width, half_width,
                            -half_height, half_height,
                            -half_depth, half_depth);
  view_matrix = ortho_matrix;

  buzz = new Buzz(shaderProgram);
}

void View::initShadersGL() {
  std::string vertex_shader_file("shaders/vshader.glsl");
  std::string fragment_shader_file("shaders/fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(toys::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(toys::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = toys::createProgramGL(shaderList);
  glUseProgram(shaderProgram);

  u_view_matrix = glGetUniformLocation(shaderProgram, "uViewMatrix");
}

void View::renderGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(u_view_matrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  buzz->render();
}