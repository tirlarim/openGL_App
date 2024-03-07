#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.hpp"
#include "Mesh.hpp"

class Model {
public:
  Model(const char* path);
  void draw(Shader &shader);
private:
  // model data
  std::vector<Mesh> meshes;
  std::vector<Texture> textures_loaded;
  std::string directory;
  bool loadModel(std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
  u32 TextureFromFile(const char *path);
};
