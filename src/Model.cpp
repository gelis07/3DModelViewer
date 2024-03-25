#include "Model.h"
#include <optional>
Model::Model(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }
    Directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
    Materials.reserve(7);
}

void Model::Draw(Shader *Program)
{
    for (auto &&mesh : Meshes){
        glm::mat4 model(1.0f);
        model = glm::translate(model, Position);
        model = glm::scale(model, Scale);
        // glm::mat4 mvp = projection* cam.view * model;
        // Program->SetUniform("mvp", mvp);
        Program->SetUniform("model", model);
        mesh.Draw(Program);
    }
}

void Model::ProcessNode(aiNode *Node, const aiScene *Scene)
{
    for (size_t i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* Mesh = Scene->mMeshes[Node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(Mesh, Scene));
    }
    for (size_t i = 0; i < Node->mNumChildren; i++)
    {
        ProcessNode(Node->mChildren[i], Scene);
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *Scene)
{
    std::vector<Vertex> Vertices;
    std::vector<GLuint> Indices;
    std::vector<Texture> Textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 Vector(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Position = Vector;
        Vector = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.Normal = Vector;
        Vector = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        vertex.Tangent = Vector;
        Vector = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
        vertex.Bitangent = Vector;
        if(mesh->mTextureCoords[0]){
            glm::vec2 vec(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.TexCoords = vec;
        }else
            vertex.TexCoords = glm::vec2(0);
        Vertices.push_back(vertex);
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
            Indices.push_back(face.mIndices[j]);
    }
    std::vector<Texture> diffuseMaps;
    std::vector<Texture> specularMaps;
    std::vector<Texture> normalMaps;
    Material* mat;
    if(mesh->mMaterialIndex >= 0)
    {

        aiMaterial* material = Scene->mMaterials[mesh->mMaterialIndex];
        bool FoundMat = false;
        for (size_t i = 0; i < LoadedMaterials.size(); i++)
        {
            if(material == LoadedMaterials[i].first){
                mat = LoadedMaterials[i].second;
                FoundMat = true;
                break;
            }
        }
        if(!FoundMat){
            diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            Materials.push_back(Material(glm::vec3(0.2f), diffuseMaps, specularMaps, normalMaps, 32.0f));
            mat = new Material(Materials.back());
            LoadedMaterials.push_back({material, mat});
        }
    }
    
    return Mesh(VertexPackage(Vertices, Indices), mat);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *Mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures = {};
    for(unsigned int i = 0; i < Mat->GetTextureCount(type); i++)
    {
        aiString str;
        Mat->GetTexture(type, i, &str);
        bool skip = false;
        for (size_t j = 0; j < LoadedTextures.size(); j++)
        {
            if(std::strcmp(LoadedTextures[j].GetPath().data(), str.C_Str()) == 0){
                textures.push_back(LoadedTextures[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            Texture texture(GL_TEXTURE_2D, std::string(str.C_Str()));
            textures.push_back(texture);
            LoadedTextures.push_back(texture);
        }
    }
    return textures;
}
