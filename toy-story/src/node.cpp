#include "node.hpp"

Node::Node(
    std::string name,
    // const GLuint &tex,
    const GLuint &shaderProgram,
    // VertexData *data,
    Node *parent) {
  this->name = name;

  this->shaderProgram = shaderProgram;

  this->parent = parent;
  // populateBuffers();
  children.resize(0);
}

void Node::populateBuffers() {
  vertex_buffer_size = data->num_vertices * sizeof(glm::vec4);
  color_buffer_size = data->num_vertices * sizeof(glm::vec4);
  tex_coord_buffer_size = data->num_vertices * sizeof(glm::vec2);
  normal_buffer_size = data->num_vertices * sizeof(glm::vec4);

  v_position = glGetAttribLocation(shaderProgram, "vPosition");
  v_color = glGetAttribLocation(shaderProgram, "vColor");
  v_texture = glGetAttribLocation(shaderProgram, "vTexture");
  v_normal = glGetAttribLocation(shaderProgram, "vNormal");
  u_model_matrix = glGetUniformLocation(shaderProgram, "uModelMatrix");
  u_normal_matrix = glGetUniformLocation(shaderProgram, "uNormalMatrix");
  u_texture_sampler = glGetUniformLocation(shaderProgram, "textureSampler");

  model_matrix = glm::mat4(1.0f);
  normal_matrix = glm::mat4(1.0f);

  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(0));
  glEnableVertexAttribArray(v_color);
  glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(vertex_buffer_size));
  glEnableVertexAttribArray(v_texture);
  glVertexAttribPointer(v_texture, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(vertex_buffer_size + color_buffer_size));
  glEnableVertexAttribArray(v_normal);
  glVertexAttribPointer(v_normal, 4, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(vertex_buffer_size + color_buffer_size +
                          tex_coord_buffer_size));

  glBufferData(GL_ARRAY_BUFFER,
              vertex_buffer_size +
                color_buffer_size +
                tex_coord_buffer_size +
                normal_buffer_size,
              NULL,
              GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER,
                  0,
                  vertex_buffer_size,
                  data->vertices);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size,
                  color_buffer_size,
                  data->colors);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size + color_buffer_size,
                  tex_coord_buffer_size,
                  data->tex_coords);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size + color_buffer_size + tex_coord_buffer_size,
                  normal_buffer_size,
                  data->normals);
}

void Node::addChild(Node *node) {
  children.push_back(node);
}

void Node::updateModelMatrix(const glm::mat4 &transformation) {
  model_matrix = transformation * model_matrix;
  normal_matrix = glm::inverse(glm::transpose(model_matrix));

  for (Node *child : children) {
    child->updateModelMatrix(transformation);
  }
}

void Node::render() {
  glBindVertexArray(vao);

  // bind texture
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(u_texture_sampler, 0);

  glUniformMatrix4fv(u_model_matrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glDrawArrays(GL_TRIANGLES, 0, data->num_vertices);

  // TODO: call child->render();
}

void Node::rotate(GLuint axis, GLfloat angle) {
  glm::mat4 rot_matrix(1.0f);
  switch (axis) {
    case 0:
      // X axis
      rot_matrix = glm::rotate(rot_matrix, glm::radians(angle), glm::vec3(1.0, 0.0, 0.0));
      break;
    case 1:
      // Y axis
      rot_matrix = glm::rotate(rot_matrix, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
      break;
    case 2:
      // Z axis
      rot_matrix = glm::rotate(rot_matrix, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
      break;
  }
  rot_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(pivot_point)) * rot_matrix * glm::translate(glm::mat4(1.0f), glm::vec3(-pivot_point));
  updateModelMatrix(rot_matrix);
}
