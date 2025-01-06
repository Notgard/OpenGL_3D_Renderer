#include "app.h"

int main(int argc, char *argv[])
{
    auto app = new App("OpenGL App");
    app->run();

    return 0;
}