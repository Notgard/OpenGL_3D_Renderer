#pragma once

#include "window.h"

#include <vector>

namespace renderer
{
    class Render
    {
    public:
        Render() : window(nullptr) {}
        virtual bool init(Window *window)
        {
            this->window = window;
            return true;
        };

        virtual void pre_render() = 0;
        virtual void post_render() = 0;
        virtual void end() = 0;

    protected:
        Window *window;
    };

    class FrameBuffer
    {
    public:
        FrameBuffer() : FBO(0), texture(0), depthbuffer(0), width(0), height(0) {};
        virtual void create_buffers(int width, int height) = 0;
        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void clear() = 0;
        virtual void delete_buffers() = 0;
        virtual unsigned int get_texture() = 0;

    protected:
        unsigned int FBO = 0;
        unsigned int texture = 0;
        unsigned int depthbuffer = 0;
        int width = 0;
        int height = 0;
    };

    class VertexBuffer {
        public:
            VertexBuffer() : VBO(0), VAO(0), IBO(0) {};
            virtual void create_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) = 0;
            virtual void bind() = 0;
            virtual void unbind() = 0;
            virtual void delete_buffers() = 0;
            virtual void draw(int index_count) = 0;
        protected:
            unsigned int VBO = 0;
            unsigned int VAO = 0;
            unsigned int IBO = 0;
    };
}
