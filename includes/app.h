#include "ogl_window.h"

class App
{
public:
    App(const std::string &name, std::vector<std::string>);
    ~App();

    void run();

private:
    OGLWindow *Awindow;
};