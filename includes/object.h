#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <memory> 

#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "mesh.h"

class ObjectHandler
{
public:
    ObjectHandler();

    ~ObjectHandler();

    bool load_object(const std::string &filename);

    void render(Shader *shader);

    double get_polygon_count();

    void toggle_object(int index);
    
    void toggle_object(int index, int value);

    void remove_object(int index);
    
private:
    struct Object
    {
        Object();

        ~Object();

        std::unique_ptr<MeshHandler> meshes;
        double polygon_count = 0;
    };

    std::vector<std::unique_ptr<Object>> objects;

    // bitfield map of objects to render, 1 = render, 0 = don't render
    std::vector<int> object_bitmap;
    double polygon_count = 0;
};