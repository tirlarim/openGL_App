#include "Model.hpp"
#include "stb_image.h"

Model::Model(const char* path) {
  loadModel(std::string(path));
}

void Model::draw(Shader &shader) {
  for (u32 i = 0; i < meshes.size(); ++i)
    meshes[i].Draw(shader);
}

bool Model::loadModel(std::string path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return false;
  }
  directory = path.substr(0, path.find_last_of('/')); // dangerous in win path strings
  processNode(scene->mRootNode, scene);
  return true;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  // process all the node's meshes (if any)
  for (u32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (u32 i = 0; i < node->mNumChildren; ++i)
    processNode(node->mChildren[i], scene); // TODO: rewrite as loop
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
  std::vector<Texture> textures;
  // process vertex positions, normals, and texture coordinates
  for (u32 i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;
    vertex.Normal.x = mesh->mNormals[i].x;
    vertex.Normal.y = mesh->mNormals[i].y;
    vertex.Normal.z = mesh->mNormals[i].z;
    if (*mesh->mTextureCoords) {
      vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
    } else vertex.TexCoords = glm::vec2(0.0f);
    vertices.push_back(vertex);
  }
  // process indices
  for (u32 i = 0; i < mesh->mNumFaces; ++i)
    for (u32 j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
      indices.push_back(mesh->mFaces[i].mIndices[j]);
  // process material
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//    std::vector<Texture> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());
  }
  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
  std::vector<Texture> textures;
  for (u32 i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (u32 j = 0; j < textures_loaded.size(); ++j) {
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str());
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture);
    }
  }
  return textures;
}

u32 Model::TextureFromFile(const char* path) {
  std::string filename = directory + '/' + path;
  u32 id;
  i32 width, height, nrChannels;
  glGenTextures(1, &id);
  u8* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    GLenum format;
    switch (nrChannels) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        format = GL_RGB;
        break;
    }
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else std::cerr << "Failed to load texture: " << filename << " -> " << width << "/" << height << std::endl;
  stbi_image_free(data);
  return id;
}
