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
  GLuint u_camera_position;
  GLuint u_light_positions;
  GLuint u_spotlight_position;
  GLuint u_lights_state;

  glm::mat4 projection_matrix;
  glm::mat4 c_rotation_matrix;
  glm::mat4 c_trans_matrix;
  glm::mat4 view_matrix;

  // Camera position and rotation Parameters
  GLfloat c_xpos, c_ypos, c_zpos;
  GLfloat c_up_x, c_up_y, c_up_z;
  GLfloat c_xrot, c_yrot, c_zrot;

  // Enable/Disable perspective view
  bool enable_perspective;

  glm::vec4 light_positions[2];
  glm::vec4 spotlight_position[2];
  GLuint lights_state[3];

  Buzz *buzz;
  int curr_node;

  void initShadersGL();
 public:
  View(GLfloat h_width, GLfloat h_height, GLfloat h_depth);
  void rotateCamera(GLuint axis, GLfloat angle);
  void zoom(GLfloat amount);
  void updateCamera();
  void renderGL();

  void rotateNode(GLuint axis, GLfloat angle);
};

#endif  // VIEW_HPP_
