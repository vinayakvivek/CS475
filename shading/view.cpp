#include "view.hpp"

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth) {
  half_width = h_width;
  half_height = h_height;
  half_depth = h_depth;

  initShadersGL();
  initBuffersGL();
}

void View::initShadersGL() {
  std::string vertex_shader_file("shaders/vshader.glsl");
  std::string fragment_shader_file("shaders/fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(viewing::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(viewing::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = viewing::createProgramGL(shaderList);
  glUseProgram(shaderProgram);
}

void View::initBuffersGL() {
  vPosition = glGetAttribLocation(shaderProgram, "vPosition");
  vColor = glGetAttribLocation(shaderProgram, "vColor");
  uModelMatrix = glGetUniformLocation(shaderProgram, "uModelMatrix");
  uViewMatrix = glGetUniformLocation(shaderProgram, "uViewMatrix");
  uPerspectiveDivide = glGetUniformLocation(shaderProgram, "uPerspectiveDivide");
  uWindowLimits = glGetUniformLocation(shaderProgram, "uWindowLimits");
  uToDCS = glGetUniformLocation(shaderProgram, "uToDCS");

  num_vao = 1;
  num_vbo = 1;
  vao = new GLuint[num_vao];
  vbo = new GLuint[num_vbo];

  glGenVertexArrays(num_vao, vao);
  glGenBuffers(num_vbo, vbo);
  glGenBuffers(num_eab, veo);

  glEnable(GL_PROGRAM_POINT_SIZE);
}

void View::renderGL() {

}