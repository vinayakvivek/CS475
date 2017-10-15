#include "node.hpp"

Node::Node(
    std::string name,
    const GLuint &tex,
    const GLuint &shaderProgram,
    GLuint num_vertices,
    glm::vec4 *vertices,
    glm::vec4 *colors,
    glm::vec2 *tex_coords,
    glm::vec4 *normals,
    Node *parent) {
  this->name = name;
  this->tex = tex;
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &this->vbo);

  this->num_vertices = num_vertices;
  this->vertices = vertices;
  this->colors = colors;
  this->tex_coords = tex_coords;
  this->normals = normals;

  this->vertex_buffer_size = num_vertices * sizeof(glm::vec4);
  this->color_buffer_size = num_vertices * sizeof(glm::vec4);
  this->tex_coord_buffer_size = num_vertices * sizeof(glm::vec2);
  this->normal_buffer_size = num_vertices * sizeof(glm::vec4);

  v_position = glGetAttribLocation(shaderProgram, "vPosition");
  v_color = glGetAttribLocation(shaderProgram, "vColor");
  v_texture = glGetAttribLocation(shaderProgram, "vTexture");
  v_normal = glGetAttribLocation(shaderProgram, "vNormal");
  u_model_matrix = glGetUniformLocation(shaderProgram, "uModelMatrix");
  u_normal_matrix = glGetUniformLocation(shaderProgram, "uNormalMatrix");

  u_texture_sampler = glGetUniformLocation(shaderProgram, "textureSampler");

  model_matrix = glm::mat4(1.0f);
  normal_matrix = glm::mat4(1.0f);

  this->parent = parent;
  populateBuffers();
  children.resize(0);
}

void Node::populateBuffers() {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // bind texture
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(u_texture_sampler, 0);

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
                  vertices);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size,
                  color_buffer_size,
                  colors);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size + color_buffer_size,
                  tex_coord_buffer_size,
                  tex_coords);
  glBufferSubData(GL_ARRAY_BUFFER,
                  vertex_buffer_size + color_buffer_size + tex_coord_buffer_size,
                  normal_buffer_size,
                  normals);
}

void Node::addChild(Node *node) {
  children.push_back(node);
}

void Node::updateModelMatrix(const glm::mat4 &transformation) {
  model_matrix *= transformation;
  normal_matrix *= glm::inverse(glm::transpose(transformation));

  for (Node *child : children) {
    child->updateModelMatrix(transformation);
  }
}

void Node::render() {
  glBindVertexArray(vao);
  glUniformMatrix4fv(u_model_matrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glDrawArrays(GL_TRIANGLES, 0, num_vertices);

  // TODO: call child->render();
}
