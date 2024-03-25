#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
    public:
        Model(const std::string& path);
        Model(const std::vector<Mesh>& meshes) : Meshes(meshes) {}
        void Draw(Shader* Program);
        inline static std::vector<Texture> LoadedTextures;
        inline static std::vector<std::pair<aiMaterial*, Material*>> LoadedMaterials;
        glm::vec3 Position, Scale;
        std::vector<Material> Materials; 
        std::vector<Mesh> Meshes;
    private:
        std::string Directory;
        void ProcessNode(aiNode* Node, const aiScene* Scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* Scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType type, std::string typeName);
};