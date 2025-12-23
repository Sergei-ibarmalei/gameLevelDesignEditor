#include "Table/app.h"

int main(int, char**)
{
    App app;
    if (!app.init()) return 1;
    app.run();
    app.shutdown();
    return 0;
}
