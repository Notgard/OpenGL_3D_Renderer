#include "ogl_window.h"

class App
{
public:
    App(const std::string &name);
    ~App();

    void run();

private:
    OGLWindow *Awindow;
};