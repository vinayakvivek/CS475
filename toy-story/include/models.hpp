#ifndef MODELS_HPP_
#define MODELS_HPP_

#include "node.hpp"
#include "texture_util.hpp"

class BuzzHead : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), 0.25f * glm::vec3(1.0f, 1.3, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI/2, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));

    local_matrix *= rotate * scale;
    updateModelMatrix(local_matrix);

    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 200 * 1.3 * 0.25 + 20.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzHead(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/face3.bmp", 256, 570);
    data = sphere(200, 30, 30);
    pivot_point = glm::vec4(0.0, 200.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    // xrot_limits[0] = -50.0; xrot_limits[1] = 30.0;
    // yrot_limits[0] = -70.0; yrot_limits[1] = 70.0;
    // zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzNeck : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 130.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzNeck(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/face3.bmp", 256, 570);
    data = cylinder(30, 30, 40);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
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
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 70.0f, 0.0f));

    local_matrix *= scale * rotate;
    updateModelMatrix(local_matrix);

    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzTorso(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(60, 100, 150);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -70.0; xrot_limits[1] = 80.0;
    yrot_limits[0] = -50.0; yrot_limits[1] = 50.0;
    zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzHip : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.0f, 0.7f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));

    local_matrix *= scale * rotate;
    updateModelMatrix(local_matrix);

    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -100.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzHip(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/hip.bmp", 768, 256);
    data = cylinder(60, 60, 70);
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
    // rotate = glm::rotate(rotate, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, 130.0f, 0.0f));

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
    yrot_limits[0] = 0.0; yrot_limits[1] = 150.0;
    zrot_limits[0] = -180.0; zrot_limits[1] = 0.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzRightUpperArm : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));
    // rotate = glm::rotate(rotate, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 130.0f, 0.0f));

    glm::mat4 initial_transformation = translate * scale * rotate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzRightUpperArm(
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
    zrot_limits[0] = -0.0; zrot_limits[1] = 180.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzLowerArm : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLowerArm(
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

class BuzzHand : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 130.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzHand(
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

class BuzzLeftThigh : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-40.0f, 0.0f, 0.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI, glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, (float)PI, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 initial_transformation = translate * scale * rotate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLeftThigh(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(40, 30, 150);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -80.0; xrot_limits[1] = 50.0;
    yrot_limits[0] = 0.0; yrot_limits[1] = 150.0;
    zrot_limits[0] = -80.0; zrot_limits[1] = 0.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class BuzzLeg : public Node {
  void setInitialTransformation() {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 150.0f, 0.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  BuzzLeg(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/buzz/suit2.bmp", 768, 512);
    data = cylinder(30, 40, 150);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    xrot_limits[0] = -0.0; xrot_limits[1] = 80.0;
    yrot_limits[0] = -0.0; yrot_limits[1] = 0.0;
    zrot_limits[0] = -0.0; zrot_limits[1] = 0.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class Buzz {
  Node *head;
  Node *torso;

  Node *neck;
  Node *hip;

  Node *left_upper_arm;
  Node *left_lower_arm;
  Node *left_hand;

  Node *right_upper_arm;
  Node *right_lower_arm;
  Node *right_hand;

  Node *left_thigh;
  Node *left_leg;

  int curr_selected_node;
 public:
  explicit Buzz(GLuint shaderProgram) {
    hip = new BuzzHip("buzz_hip", 0, shaderProgram, NULL);
    torso = new BuzzTorso("buzz_torso", 1, shaderProgram, hip);

    neck = new BuzzNeck("buzz_neck", 2, shaderProgram, torso);
    head = new BuzzHead("buzz_head", 9, shaderProgram, neck);

    left_upper_arm = new BuzzLeftUpperArm("buzz_left_upper_arm", 3, shaderProgram, torso);
    left_lower_arm = new BuzzLowerArm("buzz_left_lower_arm", 4, shaderProgram, left_upper_arm);
    left_hand = new BuzzHand("buzz_left_hand", 5, shaderProgram, left_lower_arm);

    right_upper_arm = new BuzzRightUpperArm("buzz_right_upper_arm", 6, shaderProgram, torso);
    right_lower_arm = new BuzzLowerArm("buzz_right_lower_arm", 7, shaderProgram, right_upper_arm);
    right_hand = new BuzzHand("buzz_right_hand", 8, shaderProgram, right_lower_arm);

    left_thigh = new BuzzLeftThigh("buzz_left_thigh", 10, shaderProgram, hip);
    left_leg = new BuzzLeg("buzz_left_leg", 11, shaderProgram, left_thigh);

    hip->addChild(torso);
    hip->addChild(left_thigh);
    neck->addChild(head);

    torso->addChild(neck);
    torso->addChild(left_upper_arm);
    torso->addChild(right_upper_arm);

    left_upper_arm->addChild(left_lower_arm);
    left_lower_arm->addChild(left_hand);

    right_upper_arm->addChild(right_lower_arm);
    right_lower_arm->addChild(right_hand);

    left_thigh->addChild(left_leg);

    curr_selected_node = 0;

    left_lower_arm->rotate(0, -120.0f);
    left_upper_arm->rotate(1, 90.0f);
    left_upper_arm->rotate(2, -60.0f);

    left_thigh->rotate(0, -5.0f);
    left_thigh->rotate(2, -5.0f);
    left_leg->rotate(0, 5.0f);
  }

  void render() {
    hip->render();
    torso->render();

    neck->render();
    head->render();

    left_upper_arm->render();
    left_lower_arm->render();
    left_hand->render();

    right_upper_arm->render();
    right_lower_arm->render();
    right_hand->render();

    left_thigh->render();
    left_leg->render();
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
        neck->rotate(axis, angle);
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
      case 6:
        right_upper_arm->rotate(axis, angle);
        break;
      case 7:
        right_lower_arm->rotate(axis, angle);
        break;
      case 8:
        right_hand->rotate(axis, angle);
        break;

      case 10:
        left_thigh->rotate(axis, angle);
        break;
      case 11:
        left_leg->rotate(axis, angle);
        break;
    }
  }

  void selectNode(int nodeId) {
    curr_selected_node = nodeId;
  }
};

#endif  // MODELS_HPP_
