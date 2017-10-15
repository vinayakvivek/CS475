#ifndef MODELS_HPP_
#define MODELS_HPP_

#include "node.hpp"
#include "texture_util.hpp"

class BuzzHead : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 1.0f));
    model_matrix *= scale;

    normal_matrix = glm::inverse(glm::transpose(model_matrix));
  }

 public:
  BuzzHead(
    std::string name,
    const GLuint &tex,
    const GLuint &shaderProgram,
    VertexData *data,
    Node *parent): Node(name, tex, shaderProgram, data, parent) {
    setInitialTransformation();
  }
};

class Buzz {
  Node *head;

 public:
  explicit Buzz(GLuint shaderProgram) {
    GLuint head_tex = LoadTexture("../images/all1.bmp", 256, 256);
    VertexData *data = sphere(200);

    std::cout << "num_vertices: " << data->num_vertices << "\n";

    head = new BuzzHead(
      "buzz_head",
      head_tex,
      shaderProgram,
      data,
      NULL);
  }

  void render() {
    head->render();
  }
};

#endif  // MODELS_HPP_
