#pragma once

#include "render.h"
#include "vertex.h"

#include <vector>

enum AttributeLocation {
    POSITION = 0,
    TEX_COORDS = 1,
    NORMAL = 2,
    COLOR = 3
};

namespace renderer
{
    class OGLFrameBuffer : public FrameBuffer
    {
        public:
            OGLFrameBuffer() : FrameBuffer() {};
            void create_buffers(int width, int height) override;
            void bind() override;
            void unbind() override;
            void clear() override;
            void delete_buffers() override;
            unsigned int get_texture() override;
    };

    class OGLVertexBuffer : public VertexBuffer
    {
        public:
            OGLVertexBuffer() : VertexBuffer() {};
            void create_buffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) override;
            void create_buffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
            void bind() override;
            void unbind() override;
            void delete_buffers() override;
            void draw(int index_count) override;
            void draw(GLenum draw_mode, int index_count);

            void set_color(bool is_color) { this->is_color = is_color; }
        
        private:
            bool is_color = false;
    };
}