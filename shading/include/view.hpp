#ifndef VIEW_HPP_
#define VIEW_HPP_

#include <vector>
#include <cmath>

#include "gl_framework.hpp"
#include "shader_util.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"


#define BUFFER_OFFSET(offset) (reinterpret_cast<void *>(offset))
#define PI 3.14159265
#define deg_to_rad(a) (a * PI / 180.0)

class View {
  // screen size
  GLfloat half_width;
  GLfloat half_height;
  GLfloat half_depth;

  // models setup
  std::vector<glm::vec4> points;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec4> normals;

  GLuint shaderProgram;
  GLuint *vbo, *vao;
  GLuint num_vao;
  GLuint num_vbo;

  GLuint v_position;
  GLuint v_color;
  GLuint v_normal;
  GLuint u_model_matrix;
  GLuint u_view_matrix;
  GLuint u_normal_matrix;

  glm::mat4 ortho_matrix;
  glm::mat4 model_matrix;
  glm::mat4 normal_matrix;
  glm::mat4 view_matrix;
  // glm::mat4 model_view_matrix;

  glm::mat4 cam_rotation_matrix;
  GLfloat c_xrot, c_yrot, c_zrot;
  GLfloat c_xpos, c_ypos, c_zpos;
  GLfloat c_up_x, c_up_y, c_up_z;

  void initShadersGL();
  void initBuffersGL();
  void updateCameraView();

 public:
  View(GLfloat h_width, GLfloat h_height, GLfloat h_depth);
  void renderGL();
  void addSampleTriangle();
  void addSphere(GLfloat r, GLuint n_lats = 50, GLuint n_longs = 50);
  void rotateCamera(GLuint axis, GLfloat angle);
};

#endif  // VIEW_HPP_
