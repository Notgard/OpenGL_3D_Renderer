#pragma once

#include "vertex.h"
#include "buffer.h"

#include <map>
#include <vector>
#include <iostream>

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#define INVALID_MATERIAL 0xFFFFFFFF

class MeshHandler
{
public:
    MeshHandler();

    ~MeshHandler();

    bool load_mesh(const std::string &filename);

    void render();

private:
    bool init_from_scene(const aiScene *pScene, const std::string &Filename);
    void init_mesh(unsigned int Index, const aiMesh *paiMesh);
    bool init_materials(const aiScene *pScene, const std::string &Filename);
    unsigned int load_texture(const std::string& path);
    void clear();

    struct Mesh
    {
        Mesh();

        ~Mesh();

        void init(const std::vector<Vertex>& vertices,
                  const std::vector<unsigned int>& indices);

        renderer::OGLVertexBuffer *VB;
        unsigned int numIndices;
        unsigned int materialIndex;
    };

    std::vector<Mesh> meshes;
    std::map<std::string, unsigned int> textureMap; //bind texture path to texture index
    std::map<unsigned int, unsigned int> materialTextures; //bind material index to texture index
};