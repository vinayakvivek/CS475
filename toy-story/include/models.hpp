#ifndef MODELS_HPP_
#define MODELS_HPP_

#include "node.hpp"
#include "texture_util.hpp"

class BuzzHead : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 1.3 * 0.3, 0.3f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI/2, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix *= rotate * scale;

    normal_matrix = glm::inverse(glm::transpose(model_matrix));
    pivot_point = scale * pivot_point;
  }

 public:
  BuzzHead(
    std::string name,
    const GLuint &shaderProgram,
    Node *parent): Node(name, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/face3.bmp", 256, 570);
    data = sphere(200, 30, 30);
    pivot_point = glm::vec4(0.0, -200, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzTorso : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.0f, 0.7f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -250.0f, 0.0f));
    // rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));
    GLfloat sh = 1.3f;
    glm::mat4 shear = glm::mat4(1.0f, 0.0f, sh, 0.0f,
                                sh, 1.0f, sh, 0.0f,
                                sh, 0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 1.0f);
    model_matrix *= translate * scale * rotate;

    normal_matrix = glm::inverse(glm::transpose(model_matrix));
    pivot_point = model_matrix * pivot_point;
  }

 public:
  BuzzTorso(
    std::string name,
    const GLuint &shaderProgram,
    Node *parent): Node(name, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(75, 100, 200);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    populateBuffers();
    setInitialTransformation();
  }
};

// class BuzzLeftUpperArm : public Node {

// }

class Buzz {
  Node *head;
  Node *torso;
 public:
  explicit Buzz(GLuint shaderProgram) {
    torso = new BuzzTorso("buzz_torso", shaderProgram, NULL);
    head = new BuzzHead("buzz_head", shaderProgram, torso);

    torso->addChild(head);
  }

  void render() {
    torso->render();
    head->render();
  }

  void rotate(GLuint axis, GLfloat angle) {
    torso->rotate(axis, angle);
  }
};

#endif  // MODELS_HPP_
