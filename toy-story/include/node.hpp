#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <iostream>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define BUFFER_OFFSET(offset) (reinterpret_cast<void *>(offset))
#define PI 3.14159265
#define deg_to_rad(a) (a * PI / 180.0)

class Node {
  std::string name;
  int id;

  GLuint vao, vbo, tex;

  GLuint num_vertices;
  glm::vec4 *vertices;
  glm::vec4 *colors;
  glm::vec2 *tex_coords;
  glm::vec4 *normals;

  std::size_t vertex_buffer_size;
  std::size_t color_buffer_size;
  std::size_t tex_coord_buffer_size;
  std::size_t normal_buffer_size;

  GLuint v_position;
  GLuint v_color;
  GLuint v_texture;
  GLuint v_normal;
  GLuint u_model_matrix;
  GLuint u_normal_matrix;

  // transformations
  glm::mat4 model_matrix;
  glm::mat4 normal_matrix;

  std::vector<Node*> children;
  Node *parent;

  void populateBuffers();
  // virtual void setInitialTransformation();
  // void updateScale(const glm::vec3 &scale);
  // void updateShear(GLfloat sx1, GLfloat sx2,
  //                  GLfloat sy1, GLfloat sy2,
  //                  GLfloat sz1, GLfloat sz2);

 public:
  Node(
    std::string name,
    const GLuint &tex,
    const GLuint &shaderProgram,
    GLuint num_vertices,
    glm::vec4 *vertices,
    glm::vec4 *colors,
    glm::vec2 *tex_coords,
    glm::vec4 *normals,
    Node *parent);
  void addChild(Node *node);
  void updateRotate(const glm::vec3 &rotate);
  void updateTranslation(const glm::vec3 &trans);
  void updateModelMatrix(const glm::mat4 &transformation);
  void render();
};

#endif  // _NODE_HPP_