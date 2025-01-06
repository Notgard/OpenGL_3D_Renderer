#include "buffer.h"

#include <iostream>

namespace renderer
{
    void OGLFrameBuffer::delete_buffers()
    {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &texture);
        glDeleteRenderbuffers(1, &depthbuffer);
        this->texture = 0;
        this->depthbuffer = 0;
    }

    void OGLFrameBuffer::create_buffers(int width, int height)
    {
        this->width = width;
        this->height = height;

        if (FBO != 0)
        {
            delete_buffers();
        }

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create a texture to render to
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        // Create a renderbuffer for depth and stencil testing
        glGenRenderbuffers(1, &depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

        // Check if the framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer is not complete!" << std::endl;
        }

        // Unbind the framebuffer
        this->unbind();
    }

    void OGLFrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        this->clear();
    }

    void OGLFrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGLFrameBuffer::clear()
    {
        glViewport(0, 0, this->width, this->height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    unsigned int OGLFrameBuffer::get_texture()
    {
        return this->texture;
    }

    void OGLVertexBuffer::create_buffers(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s)
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        if (is_color)
        {
            // position attribute (location = 0)
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);

            // color attribute (location = 3)
            glEnableVertexAttribArray(COLOR);
            glVertexAttribPointer(COLOR, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
        }
        else
        {
            // position attribute (location = 0)
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OGLVertexBuffer::create_buffers(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        // bind the Vertex Array Object first, then bind and set vertex buffer(s),
        // and then configure vertex attributes(s)
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // position attribute (location = 0)
        glEnableVertexAttribArray(POSITION);
        glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

        // texture attribute (location = 1)
        glEnableVertexAttribArray(TEX_COORDS);
        glVertexAttribPointer(TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

        // normal attribute (location = 2)
        glEnableVertexAttribArray(NORMAL);
        glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

        // color attribute (location = 3)
        glEnableVertexAttribArray(COLOR);
        glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OGLVertexBuffer::delete_buffers()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
    }

    void OGLVertexBuffer::bind()
    {
        glBindVertexArray(VAO);
    }

    void OGLVertexBuffer::unbind()
    {
        glBindVertexArray(0);
    }

    void OGLVertexBuffer::draw(int index_count)
    {
        this->bind();
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
        this->unbind();
    }

    void OGLVertexBuffer::draw(GLenum draw_mode, int index_count)
    {
        this->bind();
        glDrawElements(draw_mode, index_count, GL_UNSIGNED_INT, 0);
        this->unbind();
    }
}