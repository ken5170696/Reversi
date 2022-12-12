#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
public:
    // model data 
    vector<meshTexture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const& path, bool gamma = false) : gammaCorrection(gamma)
    {
        directory = path.substr(0, path.find_last_of('/'));


        string name = directory.substr(0, path.find_last_of('.'));
        string tlFileName = name + ".tl";

        if (FILE* file = fopen(tlFileName.c_str(), "r")) {
            fclose(file);
            std::cout << "[Debug] model data Found: " << tlFileName << "\n";

            //std::cout << "[Debug] loading \"textures_loaded\" \n";
            ifstream tl(tlFileName, ios::in | ios::binary);
            meshTexture tmpTl;
            while (tl.read((char*)&tmpTl, sizeof(meshTexture))) {
                //std::cout << "[Debug] push_back: " << tmpTl.id << " "  << tmpTl.type << " "  << tmpTl.path << "\n";

                meshTexture outputText = tmpTl;
                outputText.id = TextureFromFile(tmpTl.path, directory, gamma);
                textures_loaded.push_back(outputText);
            }
            tl.close();

            int idx = 0;
            string fileName = name + std::to_string(idx) + ".v";

            while (FILE* file2 = fopen(fileName.c_str(), "r")) {
                fclose(file2);

                //std::cout << "[Debug] loading mesh " + fileName << "\n";

                vector<Vertex>       vertices;
                vector<unsigned int> indices;
                vector<meshTexture>      textures;

                ifstream vert(name + std::to_string(idx) + ".v", ios::in | ios::binary);
                Vertex tmpV;
                while (vert.read((char*)&tmpV, sizeof(Vertex))) {
                    vertices.push_back(tmpV);
                    //std::cout << "[Debug] reading vertices: " << vertices.back().Position.x << " " << vertices.back().Position.y << " " << vertices.back().Position.z << "\n";
                }
                vert.close();

                ifstream indi(name + std::to_string(idx) + ".i", ios::in | ios::binary);
                unsigned int tmpI;
                while (indi.read((char*)&tmpI, sizeof(unsigned int))) {
                    indices.push_back(tmpI);
                    //std::cout << "[Debug] reading indices: " << indices.back() << "\n";
                }
                indi.close();

                /*ifstream text(name + std::to_string(idx) + ".t", ios::in | ios::binary);
                meshTexture tmpT;
                while (text.read((char*)&tmpT, sizeof(meshTexture))) {
                    textures.push_back(tmpT);
                    std::cout << "[Debug] reading meshTexture: " << TextureFromFile(tmpT.path, directory, gamma) << " " << tmpT.type << " " << tmpT.path << "\n";
                }
                text.close();*/

                meshes.push_back(Mesh(vertices, indices, textures_loaded));
                idx++;
                fileName = name + std::to_string(idx) + ".v";
            }
        }
        else {

            loadModel(path);

            ofstream text(tlFileName, ios::out | ios::binary);
            if (!text) {
                cout << "[Debug] Mesh Export Cannot open file: " + tlFileName << endl;
                return;
            }
            for (const auto& val : textures_loaded) {
                std::cout << "[Debug] textures_loaded: " << val.id << " " << val.type << " " << val.path << "\n";
                text.write(reinterpret_cast<const char*>(&val), sizeof(meshTexture));
            }
            text.close();

            for (unsigned int i = 0; i < meshes.size(); i++) {
                std::cout << "[Debug] Export model data: " + name + std::to_string(i) << "\n";
                meshes[i].exportData(name + std::to_string(i));
            }
        }

    }

    // draws the model, and thus all its meshes
    void Draw(Shader& shader) const
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path)
    {

        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<meshTexture> textures;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<meshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<meshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<meshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<meshTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<meshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<meshTexture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path, str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                meshTexture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                strcpy(texture.type, typeName.c_str());
                strcpy(texture.path, str.C_Str());
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }
};

