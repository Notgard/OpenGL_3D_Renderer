#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <memory> 

#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "mesh.h"

#include "functions.h"

struct Object
{
    Object() {
        meshes = std::make_unique<MeshHandler>();
    };

    ~Object() {
        meshes->clear();
    };

    void translate(const glm::vec3& translation) {
        obj_position += translation;
        update_model_matrix();
    }

    void scale(float scale) {
        obj_scale = scale;
        update_model_matrix();
    }

    void rotate(float angle, const glm::vec3& axis) {
        obj_rotation = glm::radians(angle);
        obj_axis = axis;
        update_model_matrix();
    }

    void update_model_matrix() {
        model = glm::mat4(1.0f); // Reset to identity matrix
        model = glm::translate(model, obj_position);
        model = glm::rotate(model, obj_rotation, obj_axis);
        model = glm::scale(model, glm::vec3(obj_scale));
    }

    glm::mat4 model = glm::mat4(1.0f);
    std::unique_ptr<MeshHandler> meshes;
    double polygon_count = 0;
    std::string name;
    float obj_scale = 1.0f;
    float obj_rotation = 0.0f;
    glm::vec3 obj_axis = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 obj_position = glm::vec3(0.0f);

    float current_scale = 1.0f;
};

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

    std::vector<std::string> get_object_name_list() {
        std::vector<std::string> names;
        for (auto &object : objects) {
            names.push_back(object->name);
        }
        return names;
    }

    std::unique_ptr<Object> &get_object(int index) {
        if (index < 0 || index >= objects.size()) {
            std::cerr << "Trying to access invalid object index: " << index << std::endl;
            return objects[0];
        }
        return objects[index];
    }
    
private:
    std::vector<std::unique_ptr<Object>> objects;

    // bitfield map of objects to render, 1 = render, 0 = don't render
    std::vector<int> object_bitmap;
    double polygon_count = 0;
};