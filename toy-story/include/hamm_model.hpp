#ifndef HAMM_MODEL_HPP_
#define HAMM_MODEL_HPP_

class HammHead : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    local_matrix = scale;
    updateModelMatrix(local_matrix);

    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 100.0f, 300.0f));
    glm::mat4 initial_transformation = translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  HammHead(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/hamm/pink.bmp", 256, 256);
    data = sphere(150, 30, 30);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    // xrot_limits[0] = -50.0; xrot_limits[1] = 30.0;
    // yrot_limits[0] = -70.0; yrot_limits[1] = 70.0;
    // zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class HammTorso : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.3f));

    local_matrix = scale;
    updateModelMatrix(local_matrix);

    // glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 200 * 1.3 * 0.25 + 20.0f, 0.0f));
    // glm::mat4 initial_transformation = translate;
    // updateModelMatrix(initial_transformation);
  }

 public:
  HammTorso(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/hamm/pink.bmp", 256, 256);
    data = sphere(250, 30, 30);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    // xrot_limits[0] = -50.0; xrot_limits[1] = 30.0;
    // yrot_limits[0] = -70.0; yrot_limits[1] = 70.0;
    // zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class HammNose : public Node {
  void setInitialTransformation() {
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    local_matrix = scale;
    updateModelMatrix(local_matrix);

    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 130.0f, 0.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), (float)PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 initial_transformation = rotate * translate;
    updateModelMatrix(initial_transformation);
  }

 public:
  HammNose(
    std::string name,
    int id,
    const GLuint &shaderProgram,
    Node *parent): Node(name, id, shaderProgram, parent) {

    tex = LoadTexture("../images/hamm/pink.bmp", 256, 256);
    data = cylinder(70, 50, 50);
    pivot_point = glm::vec4(0.0, 0.0, 0.0, 1.0);
    std::cout << "num_vertices: " << data->num_vertices << "\n";

    // xrot_limits[0] = -50.0; xrot_limits[1] = 30.0;
    // yrot_limits[0] = -70.0; yrot_limits[1] = 70.0;
    // zrot_limits[0] = -50.0; zrot_limits[1] = 50.0;

    populateBuffers();
    setInitialTransformation();
  }
};

class Hamm {
  Node *head;
  Node *torso;
  Node *nose;

  int curr_selected_node;
 public:
  explicit Hamm(GLuint shaderProgram) {
    torso = new HammTorso("hamm_torso", 0, shaderProgram, NULL);
    head = new HammHead("hamm_head", 1, shaderProgram, torso);
    nose = new HammNose("hamm_nose", 2, shaderProgram, head);

    torso->addChild(head);
    head->addChild(nose);
  }

  void render() {
    torso->render();
    head->render();
    nose->render();
  }

  void rotate(GLuint axis, GLfloat angle) {
    switch (curr_selected_node) {
      case 0:
        torso->rotate(axis, angle);
        break;
      case 1:
        head->rotate(axis, angle);
        break;
    }
  }

  void selectNode(int nodeId) {
    curr_selected_node = nodeId;
  }
};

#endif  // HAMM_MODEL_HPP_