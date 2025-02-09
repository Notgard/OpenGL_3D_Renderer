#include "app.h"

App::App(const std::string& name, std::vector<std::string> args)
{
  Awindow = new OGLWindow();
  Awindow->set_cmd_arguments(args);
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