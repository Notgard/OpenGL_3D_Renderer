#include "app.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    auto app = new App("OpenGL App", arguments);
    app->run();

    return 0;
}