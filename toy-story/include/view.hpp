#ifndef VIEW_HPP_
#define VIEW_HPP_

#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "models.hpp"

class View {
  // screen size
  GLfloat half_width;
  GLfloat half_height;
  GLfloat half_depth;

  GLuint shaderProgram;
  GLuint u_view_matrix;

  glm::mat4 ortho_matrix;
  glm::mat4 c_rotation_matrix;
  glm::mat4 c_trans_matrix;
  glm::mat4 view_matrix;

  Buzz *buzz;

  void initShadersGL();
 public:
  View(GLfloat h_width, GLfloat h_height, GLfloat h_depth);
  void renderGL();
};

#endif  // VIEW_HPP_
