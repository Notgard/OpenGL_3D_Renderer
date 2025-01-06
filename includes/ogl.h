#pragma once

#include "render.h"

#include <iostream>

namespace renderer
{
    class OGLRender : public Render
    {
    public:
        bool init(Window *window) override;
        void pre_render() override;
        void post_render() override;
        void end() override;

    private:
        typedef Render __super;
    };
}
