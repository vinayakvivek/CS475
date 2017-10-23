#ifndef MODELS_HPP_
#define MODELS_HPP_

#include "node.hpp"
#include "texture_util.hpp"

class BuzzHead : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 1.3 * 0.3, 0.3f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI/2, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));

    local_matrix *= rotate * scale;
    updateModelMatrix(local_matrix);
  }

 public:
  BuzzHead(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/face3.bmp", 256, 570);
    data = sphere(200, 30, 30);
    pivot_point = glm::vec4(0.0, -200, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -50.0; xrot_limits[1] = 30.0;
    yrot_limits[0] = -70.0; yrot_limits[1] = 70.0;
    zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzTorso : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.0f, 0.7f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -250.0f, 0.0f));

    local_matrix *= translate * scale * rotate;
    updateModelMatrix(local_matrix);
  }

 public:
  BuzzTorso(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(60, 100, 200);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -70.0; xrot_limits[1] = 80.0;
    yrot_limits[0] = -90.0; yrot_limits[1] = 90.0;
    zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzHip : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.0f, 0.7f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -300.0f, 0.0f));

    local_matrix *= translate * scale * rotate;
    updateModelMatrix(local_matrix);
  }

 public:
  BuzzHip(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(60, 60, 50);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -70.0; xrot_limits[1] = 80.0;
    yrot_limits[0] = -90.0; yrot_limits[1] = 90.0;
    zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzLeftUpperArm : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));
    // rotate = glm::rotate(rotate, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, -60.0f, 0.0f));

    glm::mat4 initial_transformation = translate * scale * rotate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLeftUpperArm(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(30, 20, 150);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -180.0; xrot_limits[1] = 40.0;
    yrot_limits[0] = -0.0; yrot_limits[1] = 0.0;
    zrot_limits[0] = -180.0; zrot_limits[1] = 00.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzLeftLowerArm : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLeftLowerArm(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(20, 15, 130);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -150.0; xrot_limits[1] = 5.0;
    yrot_limits[0] = -0.0; yrot_limits[1] = 0.0;
    zrot_limits[0] = -0.0; zrot_limits[1] = 0.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzLeftHand : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 130.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLeftHand(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(20, 20, 20);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -90.0; xrot_limits[1] = 80.0;
    yrot_limits[0] = -0.0; yrot_limits[1] = 0.0;
    zrot_limits[0] = -0.0; zrot_limits[1] = 0.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class Buzz {
  Node *head;
  Node *torso;
  Node *hip;

  Node *left_upper_arm;
  Node *left_lower_arm;
  Node *left_hand;

  int curr_selected_node;
 public:
  explicit Buzz(GLuint shaderProgram) {
    hip = new BuzzHip("buzz_hip", 0, shaderProgram, NULL);
    torso = new BuzzTorso("buzz_torso", 1, shaderProgram, hip);
    head = new BuzzHead("buzz_head", 2, shaderProgram, torso);
    left_upper_arm = new BuzzLeftUpperArm("buzz_left_upper_arm", 3, shaderProgram, torso);
    left_lower_arm = new BuzzLeftLowerArm("buzz_left_lower_arm", 4, shaderProgram, left_upper_arm);
    left_hand = new BuzzLeftHand("buzz_left_hand", 5, shaderProgram, left_lower_arm);

    hip->addChild(torso);
    torso->addChild(head);
    torso->addChild(left_upper_arm);
    left_upper_arm->addChild(left_lower_arm);
    left_lower_arm->addChild(left_hand);
    curr_selected_node = 0;
  }

  void render() {
    hip->render();
    torso->render();
    head->render();
    left_upper_arm->render();
    left_lower_arm->render();
    left_hand->render();
  }

  void rotate(GLuint axis, GLfloat angle) {
    switch (curr_selected_node) {
      case 0:
        hip->rotate(axis, angle);
        break;
      case 1:
        torso->rotate(axis, angle);
        break;
      case 2:
        head->rotate(axis, angle);
        break;
      case 3:
        left_upper_arm->rotate(axis, angle);
        break;
      case 4:
        left_lower_arm->rotate(axis, angle);
        break;
      case 5:
        left_hand->rotate(axis, angle);
        break;
    }
  }

  void selectNode(int nodeId) {
    curr_selected_node = nodeId;
  }
};

#endif  // MODELS_HPP_
