#pragma once

#include <string>
#include <vector>
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "Shader.hpp"
#include "types.h"

//#define MAX_BONE_INFLUENCE 4

//struct Vertex {
//  // position
//  glm::vec3 Position;
//  // normal
//  glm::vec3 Normal;
//  // texCoords
//  glm::vec2 TexCoords;
//  // tangent
//  glm::vec3 Tangent;
//  // bitangent
//  glm::vec3 Bitangent;
//  //bone indexes which will influence this vertex
//  int m_BoneIDs[MAX_BONE_INFLUENCE];
//  //weights from each bone
//  float m_Weights[MAX_BONE_INFLUENCE];
//};

struct Vertex { // maybe I should to set default value to zero?
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  u32 id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  // Mesh data
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
  std::vector<Texture> textures;
  Mesh(const std::vector<Vertex>& vertices, const std::vector<u32>& indices, const std::vector<Texture>& textures);
  void Draw(Shader &shader);
private:
  // Render data
  u32 VAO, VBO, EBO;
  void setupMesh();
};
