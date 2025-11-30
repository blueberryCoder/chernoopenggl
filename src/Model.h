//
// Created by blueberry on 2025/10/3.
//

#pragma once
#include "GLStructures.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

class Model {
public:
    Model(const std::string path) {
        loadModel(path);
    }

    void Draw(Shader &shader, unsigned int count = 0);

    vector<Mesh>& GetMeshes();

private:
    // model data
    vector<Mesh> meshes;
    // cache
    vector<shared_ptr<Texture> > textures_loaded;
    string directory;

    void loadModel(string path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<shared_ptr<Texture> > loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                                      string typeName);
};
