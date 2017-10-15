#include "view.hpp"

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth) {
  half_width = h_width;
  half_height = h_height;
  half_depth = h_depth;

  c_xpos = 0.0; c_ypos = 0.0; c_zpos = 300.0;
  c_up_x = 0.0; c_up_y = 1.0; c_up_z = 0.0;
  c_xrot = 0.0; c_yrot = 0.0; c_zrot = 0.0;

  enable_perspective = true;

  light_positions[0] = glm::vec4(400.0, 400.0, 400.0, 1.0);
  light_positions[1] = glm::vec4(-400.0, -400.0, -400.0, 1.0);

  spotlight_position[0] = glm::vec4(0.0, 0.0, 300.0, 1.0);
  spotlight_position[1] = glm::vec4(0.0, 0.0, 0.0, 1.0);

  lights_state[0] = 0;
  lights_state[1] = 0;
  lights_state[2] = 1;

  initShadersGL();

  updateCamera();
  buzz = new Buzz(shaderProgram);
}

void View::initShadersGL() {
  std::string vertex_shader_file("shaders/vshader.glsl");
  std::string fragment_shader_file("shaders/fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(toys::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(toys::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = toys::createProgramGL(shaderList);
  glUseProgram(shaderProgram);

  u_view_matrix = glGetUniformLocation(shaderProgram, "uViewMatrix");
  u_camera_position = glGetUniformLocation(shaderProgram, "uCameraPosition");
  u_light_positions = glGetUniformLocation(shaderProgram, "uLightPositions");
  u_spotlight_position = glGetUniformLocation(shaderProgram, "uSpotLightPosition");
  u_lights_state = glGetUniformLocation(shaderProgram, "uLightsState");
}

void View::renderGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniform4fv(u_camera_position, 1, glm::value_ptr(glm::vec4(c_xpos, c_ypos, c_zpos, 1.0)));
  glUniform4fv(u_light_positions, 2, glm::value_ptr(light_positions[0]));
  glUniform4fv(u_spotlight_position, 2, glm::value_ptr(spotlight_position[0]));
  glUniform1uiv(u_lights_state, 3, &lights_state[0]);
  glUniformMatrix4fv(u_view_matrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  buzz->render();
}

void View::updateCamera() {
  // Creating the lookAt and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * c_rotation_matrix;

  // Creating the lookAt matrix
  glm::mat4 lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));

  // creating the projection matrix
  if (enable_perspective)
    projection_matrix = glm::frustum(-100.0f, 100.0f, -100.0f, 100.0f, 100.0f, -half_depth);
    // projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 50.0f, 250.0f);
  else
    projection_matrix = glm::ortho(-half_width, half_width,
                            -half_height, half_height,
                            -half_depth, half_depth);

  view_matrix = projection_matrix * lookat_matrix;
}

void View::rotateCamera(GLuint axis, GLfloat angle) {
  switch (axis) {
    case 0:
      // X axis
      c_xrot += angle;
      break;
    case 1:
      // Y axis
      c_yrot += angle;
      break;
    case 2:
      // Z axis
      c_zrot += angle;
      break;
  }

  updateCamera();
}

void View::zoom(GLfloat amount) {
  c_zpos -= amount;
  updateCamera();
}