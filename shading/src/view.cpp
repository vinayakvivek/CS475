#include "view.hpp"

#include <string>

View::View(GLfloat h_width, GLfloat h_height, GLfloat h_depth) {
  half_width = h_width;
  half_height = h_height;
  half_depth = h_depth;

  initShadersGL();
  initBuffersGL();

  c_xrot = 0.0; c_yrot = 0.0; c_zrot = 0.0;
  c_xpos = 0.0; c_ypos = 0.0; c_zpos = 200.0;
  c_up_x = 0.0; c_up_y = 1.0; c_up_z = 0.0;

  xrot = 0.0; yrot = 0.0; zrot = 0.0;

  ortho_matrix = glm::ortho(-half_width, half_width,
                            -half_height, half_height,
                            -half_depth, half_depth);
  model_matrix = glm::mat4(1.0f);
  rotation_matrix = glm::mat4(1.0f);
  scale_matrix = glm::mat4(1.0f);
  normal_matrix = glm::mat4(1.0f);
  cam_rotation_matrix = glm::mat4(1.0f);
  updateCameraView();
}

void View::initShadersGL() {
  std::string vertex_shader_file("shaders/vshader.glsl");
  std::string fragment_shader_file("shaders/fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(mynamespace::loadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(mynamespace::loadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = mynamespace::createProgramGL(shaderList);
  glUseProgram(shaderProgram);
}

void View::initBuffersGL() {
  v_position = glGetAttribLocation(shaderProgram, "vPosition");
  v_color = glGetAttribLocation(shaderProgram, "vColor");
  v_normal = glGetAttribLocation(shaderProgram, "vNormal");
  v_tex = glGetAttribLocation(shaderProgram, "vTex");
  v_tangent = glGetAttribLocation(shaderProgram, "vTangent");
  u_model_matrix = glGetUniformLocation(shaderProgram, "uModelMatrix");
  u_view_matrix = glGetUniformLocation(shaderProgram, "uViewMatrix");
  u_normal_matrix = glGetUniformLocation(shaderProgram, "uNormalMatrix");

  num_vao = 1;
  num_vbo = 1;
  vao = new GLuint[num_vao];
  vbo = new GLuint[num_vbo];

  // Load Textures
  tex = LoadTexture("../images/all1.bmp", 256, 256);
  nmap = LoadTexture("../images/NormalMap.bmp", 512, 512);
  // tex = LoadTexture("../images/buzz/face_mask.bmp", 436, 591);

  // glBindTexture(GL_TEXTURE_2D, tex);

  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 0);

  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, nmap);
  glUniform1i(glGetUniformLocation(shaderProgram, "normalMapSampler"), 1);


  glGenVertexArrays(num_vao, vao);
  glGenBuffers(num_vbo, vbo);

  glEnable(GL_PROGRAM_POINT_SIZE);
}

void View::renderGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray(vao[0]);
  glUniformMatrix4fv(u_model_matrix, 1, GL_FALSE, glm::value_ptr(model_matrix));
  glUniformMatrix4fv(u_view_matrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniformMatrix4fv(u_normal_matrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));
  glDrawArrays(GL_TRIANGLES, 0, points.size());

  // wire frame
  // glDrawArrays(GL_LINES, 0, points.size());
}

void View::addSampleTriangle() {
  points = std::vector<glm::vec4>({
    glm::vec4(0.0, 0.0, 0.0, 1.0),
    glm::vec4(-200.0, 0.0, 0.0, 1.0),
    glm::vec4(0.0, 200.0, 0.0, 1.0)
  });

  colors = std::vector<glm::vec4>({
    glm::vec4(0.2, 0.4, 0.6, 1.0),
    glm::vec4(0.5, 0.1, 0.4, 1.0),
    glm::vec4(0.7, 0.9, 0.1, 1.0),
  });

  assert(points.size() == colors.size());

  GLuint buffer_len = points.size() * sizeof(glm::vec4);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(v_color);
  glVertexAttribPointer(v_color, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_len));

  glBufferData(GL_ARRAY_BUFFER, 2 * buffer_len, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_len, &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, buffer_len, buffer_len, &colors[0]);
}

glm::vec2 UV(GLfloat x, GLfloat y, GLfloat z) {
  glm::vec3 n = glm::normalize(glm::vec3(x, y, z));
  GLfloat u = atan2(n.x, n.z) / (PI) + 0.5;
  GLfloat v = n.y * 0.5 + 0.5;

  // if (z < 40) {
  //   u = v = -1;
  // }

  return glm::vec2(u, v);
}

glm::vec3 getTangent(GLfloat theta, GLfloat phi) {
  theta -= PI/2;
  phi += PI;
  return glm::normalize(glm::vec3(
      cos(phi) * sin(theta),
      sin(phi) * sin(theta),
      cos(theta)));
}

void View::addSphere(GLfloat r, GLuint n_lats, GLuint n_longs) {
  GLfloat slice_length = (PI / n_lats);
  GLfloat sector_length = (2 * PI / n_longs);

  GLfloat theta, phi, x, y, z;
  points.resize(0);
  normals.resize(0);
  texCoords.resize(0);
  tangents.resize(0);

  for (int i = 0; i < n_lats; ++i) {
    for (int j = 0; j < n_longs; ++j) {
      theta = i * slice_length;
      phi = j * sector_length;

      x = r * cos(phi) * sin(theta);
      y = r * sin(phi) * sin(theta);
      z = r * cos(theta);
      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      // texCoords.push_back(UV(x, y, z));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));

      theta += slice_length;
      x = r * cos(phi) * sin(theta);
      y = r * sin(phi) * sin(theta);
      z = r * cos(theta);
      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));
      // texCoords.push_back(UV(x, y, z));

      phi += sector_length;
      x = r * cos(phi) * sin(theta);
      y = r * sin(phi) * sin(theta);
      z = r * cos(theta);
      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));
      // texCoords.push_back(UV(x, y, z));

      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));
      // texCoords.push_back(UV(x, y, z));

      theta -= slice_length;
      x = r * cos(phi) * sin(theta);
      y = r * sin(phi) * sin(theta);
      z = r * cos(theta);
      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));
      // texCoords.push_back(UV(x, y, z));

      phi -= sector_length;
      x = r * cos(phi) * sin(theta);
      y = r * sin(phi) * sin(theta);
      z = r * cos(theta);
      points.push_back(glm::vec4(x, y, z, 1.0));
      normals.push_back(glm::vec4(x, y, z, 0.0));
      texCoords.push_back(glm::vec2(theta / PI, phi / (2 * PI)));
      tangents.push_back(getTangent(theta, phi));
      // texCoords.push_back(UV(x, y, z));
    }
  }

  // scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.5, 1.0));
  // model_matrix = scale_matrix;

  GLuint points_buffer_len = points.size() * sizeof(glm::vec4);
  GLuint normals_buffer_len = normals.size() * sizeof(glm::vec4);
  GLuint texCoords_buffer_len = texCoords.size() * sizeof(glm::vec2);
  GLuint tangets_buffer_len = tangents.size() * sizeof(glm::vec3);

  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  glEnableVertexAttribArray(v_position);
  glVertexAttribPointer(v_position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(v_normal);
  glVertexAttribPointer(v_normal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_len));
  glEnableVertexAttribArray(v_tex);
  glVertexAttribPointer(v_tex, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_len + normals_buffer_len));
  glEnableVertexAttribArray(v_tangent);
  glVertexAttribPointer(v_tangent, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points_buffer_len + normals_buffer_len + texCoords_buffer_len));


  glBufferData(GL_ARRAY_BUFFER, (points_buffer_len + normals_buffer_len + texCoords_buffer_len + tangets_buffer_len), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, points_buffer_len, &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, points_buffer_len, normals_buffer_len, &normals[0]);
  glBufferSubData(GL_ARRAY_BUFFER, points_buffer_len + normals_buffer_len, texCoords_buffer_len, &texCoords[0]);
  glBufferSubData(GL_ARRAY_BUFFER, points_buffer_len + normals_buffer_len + texCoords_buffer_len, tangets_buffer_len, &tangents[0]);
}

void View::rotateCamera(GLuint axis, GLfloat angle) {
  angle = deg_to_rad(angle);
  switch (axis) {
    case 0:
      // x-axis rotation
      c_xrot += angle;
      break;
    case 1:
      c_yrot += angle;
      break;
    case 2:
      c_zrot += angle;
      break;
  }

  cam_rotation_matrix = glm::rotate(glm::mat4(1.0f), c_xrot, glm::vec3(1.0f, 0.0f, 0.0f));
  cam_rotation_matrix = glm::rotate(cam_rotation_matrix, c_yrot, glm::vec3(0.0f, 1.0f, 0.0f));
  cam_rotation_matrix = glm::rotate(cam_rotation_matrix, c_zrot, glm::vec3(0.0f, 0.0f, 1.0f));

  updateCameraView();
}

void View::rotate(GLuint axis, GLfloat angle) {
  angle = deg_to_rad(angle);
  switch (axis) {
    case 0:
      // x-axis rotation
      xrot += angle;
      break;
    case 1:
      yrot += angle;
      break;
    case 2:
      zrot += angle;
      break;
  }

  rotation_matrix = glm::rotate(glm::mat4(1.0f), xrot, glm::vec3(1.0f, 0.0f, 0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
  rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0.0f, 0.0f, 1.0f));

  model_matrix =  rotation_matrix * scale_matrix;
  normal_matrix = glm::transpose(glm::inverse(model_matrix));
}

void View::updateCameraView() {
  glm::vec4 c_pos = glm::vec4(c_xpos, c_ypos, c_zpos, 1.0) * cam_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x, c_up_y, c_up_z, 1.0) * cam_rotation_matrix;
  glm::mat4 lookat_matrix = glm::lookAt(glm::vec3(c_pos), glm::vec3(0.0), glm::vec3(c_up));
  // glm::mat4 projection_matrix = glm::frustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);

  view_matrix = ortho_matrix * lookat_matrix;
}