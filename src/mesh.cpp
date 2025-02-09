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

bool MeshHandler::load_mesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    // clear();

    std::cout << "Loading mesh: " << filename << std::endl;

    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    const aiScene *scene = importer.ReadFile(filename.c_str(), flags);

    if (scene == nullptr)
    {
        fprintf(stderr, "Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
        return false;
    }

    return init_from_scene(scene, filename);
}

bool MeshHandler::init_from_scene(const aiScene *scene, const std::string &filename)
{
    std::cout << "Initializing scene" << std::endl;

    meshes.reserve(scene->mNumMeshes);
    unsigned int nb_meshes = scene->mNumMeshes;

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < nb_meshes; i++)
    {
        const aiMesh *paiMesh = scene->mMeshes[i];
        meshes.push_back(std::make_unique<Mesh>());
        init_mesh(i, paiMesh);
    }

    return init_materials(scene, filename);
}

void MeshHandler::init_mesh(unsigned int index, const aiMesh *paiMesh)
{
    //std::cout << "Initializing mesh: " << index << std::endl;

    assert(index < meshes.size());

    meshes[index] = std::make_unique<Mesh>(); // Use smart pointer

    meshes[index]->materialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    vertices.reserve(paiMesh->mNumVertices);
    std::vector<unsigned int> indices;
    indices.reserve(paiMesh->mNumFaces * 3);

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    //std::cout << "Pushing vertices" << std::endl;
    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D *pPos = &(paiMesh->mVertices[i]);
        const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
        // if no color is found, default to white
        const aiColor4D color = paiMesh->HasVertexColors(0) ? paiMesh->mColors[0][i] : aiColor4D(1.0f);

        Vertex v({pPos->x, pPos->y, pPos->z},
                 {pTexCoord->x, pTexCoord->y},
                 {pNormal->x, pNormal->y, pNormal->z},
                 {color.r, color.g, color.b, color.a});

        vertices.push_back(v);
    }
    //std::cout << "Pushing indices" << std::endl;
    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace &Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        polygon_count++;
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    std::cout << "Polygon count for mesh: " << index << " is " << polygon_count << std::endl;
    //std::cout << "Initializing mesh: " << index << " with " << vertices.size() << " vertices and " << indices.size() << " indices" << std::endl;
    meshes[index]->init(vertices, indices);
}

void MeshHandler::save_texture_data(TextureType type, std::string& fullPath, unsigned int i)
{
    std::cout << "Texture found for material: " << i << "at path: " << fullPath << std::endl;

    // Check if the texture has already been loaded
    if (textureMap.find(fullPath) == textureMap.end())
    {
        textureMap[fullPath] = load_texture(fullPath);
    }

    materialTextures[i] = textureMap[fullPath];
    textureToType[materialTextures[i]] = type;
}

bool MeshHandler::init_materials(const aiScene *scene, const std::string &filename)
{
    std::cout << "Initializing materials" << std::endl;
    if (!scene->HasMaterials())
    {
        std::cerr << "No materials found in scene" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Materials found in scene: " << scene->mNumMaterials << std::endl;
    }

    std::string directory = filename.substr(0, filename.find_last_of('/'));

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial *pMaterial = scene->mMaterials[i];
        aiString material_name;
        unsigned int num_textures = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
        unsigned int num_spec = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
        unsigned int num_norm = pMaterial->GetTextureCount(aiTextureType_NORMALS);
        unsigned int num_ambi = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
        unsigned int num_emis = pMaterial->GetTextureCount(aiTextureType_EMISSIVE);
        unsigned int num_height = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
        unsigned int num_shine = pMaterial->GetTextureCount(aiTextureType_SHININESS);
        unsigned int num_opac = pMaterial->GetTextureCount(aiTextureType_OPACITY);
        unsigned int num_disp = pMaterial->GetTextureCount(aiTextureType_DISPLACEMENT);
        unsigned int num_light = pMaterial->GetTextureCount(aiTextureType_LIGHTMAP);
        unsigned int num_refl = pMaterial->GetTextureCount(aiTextureType_REFLECTION);
        unsigned int num_unknown = pMaterial->GetTextureCount(aiTextureType_UNKNOWN);
        // put these in a vector, iterate through each element and only print the value if it is greater than 0
        texture_counts = {num_textures, num_spec, num_norm, num_height, num_ambi, num_emis, num_shine, num_opac, num_disp, num_light, num_refl, num_unknown};
        for (unsigned int j = 0; j < texture_counts.size(); j++)
        {
            if (texture_counts[j] > 0)
            {
                std::cout << "Material: " << i << " Texture type: " << texture_info[j] << " count: " << texture_counts[j] << std::endl;
            }
        }

        if (pMaterial->Get(AI_MATKEY_NAME, material_name) != AI_SUCCESS)
        {
            std::cout << "failed to find material name" << std::endl;
            material_name = "";
        }

        if (num_textures > 0)
        {
            aiString Path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
            {
                std::string fullPath = directory + "/" + Path.C_Str();
                save_texture_data(DIFFUSE, fullPath, i);
            }
            else if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &Path) == AI_SUCCESS)
            {
                std::string fullPath = directory + "/" + Path.C_Str();
                save_texture_data(SPECULAR, fullPath, i);
            }
            else if (pMaterial->GetTexture(aiTextureType_HEIGHT, 0, &Path) == AI_SUCCESS)
            {
                std::string fullPath = directory + "/" + Path.C_Str();
                save_texture_data(HEIGHT, fullPath, i);
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

void MeshHandler::render(Shader *shader)
{
    glEnableVertexAttribArray(POSITION);   // Position
    glEnableVertexAttribArray(TEX_COORDS); // UV
    glEnableVertexAttribArray(NORMAL);     // Normal
    glEnableVertexAttribArray(COLOR);      // Color

    hasTextures = materialTextures.size() > 0;
    if (!hasTextures)
    {
        shader->set_int("textureMapping", 0);
    }

    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int textureID = 0;
        unsigned int textureType = 0;
        unsigned int material_index = meshes[i]->materialIndex;

        // Check if the material has a texture
        if (materialTextures.find(material_index) != materialTextures.end())
        {
            textureID = materialTextures[material_index];
            textureType = textureToType[textureID];
            switch (textureType)
            {
            case DIFFUSE:
                if (texture_counts[DIFFUSE] < 1)
                {
                    shader->set_int("hasDiffuse", 0);
                    break;
                }
                // bind DIFFUSE
                glActiveTexture(GL_TEXTURE0); // activate texture 1 (diffuse)
                glBindTexture(GL_TEXTURE_2D, textureID);
                shader->set_int(std::string(TEXTURE_DIFFUSE), DIFFUSE);
                break;
            case SPECULAR:
                if (texture_counts[SPECULAR] < 1)
                {
                    shader->set_int("hasSpecex", 0);
                    break;
                }
                // bind SPECULAR
                glActiveTexture(GL_TEXTURE1); // activate texture 2 (specular)
                glBindTexture(GL_TEXTURE_2D, textureID);
                shader->set_int(std::string(TEXTURE_SPECULAR), SPECULAR);
                break;
            case NORMALS:
            case HEIGHT:
                if (texture_counts[HEIGHT] < 1 || texture_counts[NORMALS] < 1)
                {
                    shader->set_int("hasNormal", 0);
                    break;
                }
                // bind NORMALS
                glActiveTexture(GL_TEXTURE2); // activate texture 3 (normals)
                glBindTexture(GL_TEXTURE_2D, textureID);
                shader->set_int(std::string(TEXTURE_NORMAL), NORMALS);
                break;
            }
        }

        // Draw the mesh
        if (!meshes[i])
            continue; // Skip invalid meshes

        if (meshes[i]->VB)
        {
            meshes[i]->VB->draw(meshes[i]->numIndices);
        }
    }

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(TEX_COORDS);
    glDisableVertexAttribArray(NORMAL);
    glDisableVertexAttribArray(COLOR);
}
