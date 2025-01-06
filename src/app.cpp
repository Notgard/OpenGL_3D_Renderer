#include "app.h"

App::App(const std::string& name)
{
  Awindow = new OGLWindow();
  Awindow->init(1024, 720, name);

}

App::~App()
{
  delete Awindow;
}

void App::run()
{
  while (Awindow->is_running())
  {
    Awindow->render();
  }
}