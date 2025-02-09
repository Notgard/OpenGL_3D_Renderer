#pragma once

#include "vertex.h"
#include "buffer.h"

#include <map>
#include <vector>
#include <iostream>
#include <memory> 

// assimp include files. These three are usually needed.
#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "shader.h"

#define INVALID_MATERIAL 0xFFFFFFFF

enum TextureType {
    DIFFUSE = 0,
    SPECULAR = 1,
    NORMALS = 2,
    HEIGHT = 3,
    AMBIENT = 4,
    EMISSIVE = 5,
    SHININESS = 6,
    OPACITY = 7,
    DISPLACEMENT = 8,
    LIGHTMAP = 9,
    REFLECTION = 10,
    UNKNOWN = 11
};

class MeshHandler
{
public:
    MeshHandler();

    ~MeshHandler();

    bool load_mesh(const std::string &filename);

    void render(Shader *shader);

    void clear();

    double get_polygon_count() { return polygon_count; }

    private:
    bool init_from_scene(const aiScene *pScene, const std::string &Filename);
    void init_mesh(unsigned int Index, const aiMesh *paiMesh);
    bool init_materials(const aiScene *pScene, const std::string &Filename);
    unsigned int load_texture(const std::string &path);
    void save_texture_data(TextureType type, std::string& fullPath, unsigned int i);

    struct Mesh
    {
        Mesh();

        ~Mesh();

        void init(const std::vector<Vertex> &vertices,
                  const std::vector<unsigned int> &indices);

        renderer::OGLVertexBuffer *VB;
        unsigned int numIndices;
        unsigned int materialIndex;
    };

    std::vector<std::unique_ptr<Mesh>> meshes;
    std::map<std::string, unsigned int> textureMap;        // bind texture path to texture index
    std::map<unsigned int, unsigned int> materialTextures; // bind material index to texture index
    std::map<unsigned int, unsigned int> textureToType;    // bind texture index to texture type
    bool hasTextures = false;

    std::vector<unsigned int> texture_counts;
    double polygon_count = 0;

    std::map<unsigned int, std::string> texture_info = {
        {0, "DIFFUSE"},
        {1, "SPECULAR"},
        {2, "NORMALS"},
        {3, "HEIGHT"},
        {4, "AMBIENT"},
        {5, "EMISSIVE"},
        {6, "SHININESS"},
        {7, "OPACITY"},
        {8, "DISPLACEMENT"},
        {9, "LIGHTMAP"},
        {10, "REFLECTION"},
        {11, "UNKNOWN"}};
};