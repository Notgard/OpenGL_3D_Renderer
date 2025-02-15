#include "object.h"

ObjectHandler::ObjectHandler()
{
}

ObjectHandler::~ObjectHandler()
{
    for (auto &object : objects)
    {
        object->meshes->clear();
        //safe_delete(object);
    }
    objects.clear();
}

void ObjectHandler::toggle_object(int index)
{
    if (index < 0 || index >= object_bitmap.size()) {
        std::cerr << "Trying to toggle invalid object index: " << index << std::endl;
        return;
    }
    object_bitmap[index] = !object_bitmap[index];
}

void ObjectHandler::toggle_object(int index, int value)
{
    if (index < 0 || index >= object_bitmap.size()) {
        std::cerr << "Trying to toggle invalid object index: " << index << "with value: " << value << std::endl;
        return;
    }
    object_bitmap[index] = value;
}

void ObjectHandler::remove_object(int index)
{
    if (index < 0 || index >= objects.size()) {
        std::cerr << "Trying to remove invalid object index: " << index << std::endl;
        return;
    }
    objects[index]->meshes->clear();
    //safe_delete(objects[index]);
    objects.erase(objects.begin() + index);
}

double ObjectHandler::get_polygon_count() { return this->polygon_count; }

bool ObjectHandler::load_object(const std::string &filename)
{
    std::unique_ptr<Object> new_object = std::make_unique<Object>();
    if (!new_object) {
        std::cerr << "Failed to allocate memory for new object." << std::endl;
        return false;
    }
    if (!new_object->meshes->load_mesh(filename))
    {
        return false;
    }
    std::cout << "Object loaded: " << filename << std::endl;
    new_object->name = getFileName(filename);
    object_bitmap.push_back(SHOW_OBJECT);
    new_object->polygon_count = new_object->meshes->get_polygon_count();
    //std::cout << "Polygon count: " << new_object->polygon_count << std::endl;
    this->polygon_count += new_object->polygon_count;
    //std::cout << "Total polygon count: " << this->polygon_count << std::endl;
    objects.push_back(std::move(new_object));
    return true;
}

void ObjectHandler::render(Shader *shader)
{
    if (!shader) {
        std::cerr << "Shader is null!" << std::endl;
        return;
    }
    //std::cout << "Rendering objects " << objects.size() << std::endl;
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        if (object_bitmap[i] != HIDE_OBJECT)
        {
            //std::cout << "Rendering object: " << i << std::endl;
            shader->set_model(objects[i]->model);
            objects[i]->meshes->render(shader);
        }
    }
}