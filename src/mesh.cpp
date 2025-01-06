#include "config.h"
#include "mesh.h"

#ifdef _MSC_VER
#pragma warning(disable : 4100) // Disable warning 'unreferenced formal parameter'
#endif                          // _MSC_VER

#ifdef _MSC_VER
#pragma warning(default : 4100) // Enable warning 'unreferenced formal parameter'
#endif                          // _MSC_VER

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

MeshHandler::Mesh::Mesh()
{
    VB = nullptr;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;
}

MeshHandler::Mesh::~Mesh()
{
    if (VB != nullptr)
    {
        VB->delete_buffers();
        safe_delete(VB);
    }
}

void MeshHandler::Mesh::init(const std::vector<Vertex> &vertices,
                             const std::vector<unsigned int> &indices)
{
    numIndices = indices.size();

    VB = new renderer::OGLVertexBuffer();
    VB->create_buffers(vertices, indices);
}

MeshHandler::MeshHandler()
{
}

MeshHandler::~MeshHandler()
{
    clear();
}

void MeshHandler::MeshHandler::clear()
{
    meshes.clear();
    textureMap.clear();
    materialTextures.clear();
}

bool MeshHandler::load_mesh(const std::string &filename)
{
    // Release the previously loaded mesh (if it exists)
    // clear();

    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    const aiScene *scene = importer.ReadFile(filename.c_str(), flags);

    if (scene == nullptr)
    {
        fprintf(stderr, "Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
        return false;
    }

    if (!init_from_scene(scene, filename))
    {
        return false;
    }

    return true;
}

bool MeshHandler::init_from_scene(const aiScene *scene, const std::string &filename)
{
    meshes.resize(scene->mNumMeshes);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        const aiMesh *paiMesh = scene->mMeshes[i];
        init_mesh(i, paiMesh);
    }

    return init_materials(scene, filename);
}

void MeshHandler::init_mesh(unsigned int index, const aiMesh *paiMesh)
{
    meshes[index].materialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D *pPos = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        //if no color is found, default to white
        const aiColor4D color = paiMesh->HasVertexColors(0) ? paiMesh->mColors[0][i] : aiColor4D(1.0f);

        Vertex v({pPos->x, pPos->y, pPos->z},
                 {pTexCoord->x, pTexCoord->y},
                 {pNormal->x, pNormal->y, pNormal->z},
                 {color.r, color.g, color.b, color.a});

        vertices.push_back(v);
    }
    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace &Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }
    meshes[index].init(vertices, indices);
}

bool MeshHandler::init_materials(const aiScene *scene, const std::string &filename)
{
    if (!scene->HasMaterials()) {
        std::cerr << "No materials found in scene" << std::endl;
        return false;
    }
    
    std::string directory = filename.substr(0, filename.find_last_of('/'));

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial *pMaterial = scene->mMaterials[i];

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
            {
                std::string fullPath = directory + "/" + Path.C_Str();
                std::cout << "Texture found for material: " << i << " at path: " << fullPath << std::endl;

                // Check if the texture has already been loaded
                if (textureMap.find(fullPath) == textureMap.end())
                {
                    textureMap[fullPath] = load_texture(fullPath);
                }

                materialTextures[i] = textureMap[fullPath];
            }
            else
            {
                std::cout << "No texture found for material: " << i << std::endl;
            }
        }
        else
        {
            // TODO: Assign default texture for materials with no diffuse map
        }
    }

    return true;
}

unsigned int MeshHandler::load_texture(const std::string &path)
{
    int x, y, n;
    unsigned char *data = stbi_load(path.c_str(), &x, &y, &n, STBI_rgb_alpha);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }

    unsigned int texId;

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texId;
}

void MeshHandler::render()
{
    glEnableVertexAttribArray(POSITION); // Position
    glEnableVertexAttribArray(TEX_COORDS); // UV
    glEnableVertexAttribArray(NORMAL); // Normal
    glEnableVertexAttribArray(COLOR); // Color

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int textureID = 0;
        unsigned int material_index = meshes[i].materialIndex;

        // Check if the material has a texture
        if (materialTextures.find(material_index) != materialTextures.end())
        {
            textureID = materialTextures[material_index];
        }

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Draw the mesh
        meshes[i].VB->draw(meshes[i].numIndices);
    }

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(TEX_COORDS);
    glDisableVertexAttribArray(NORMAL);
    glDisableVertexAttribArray(COLOR);
}
