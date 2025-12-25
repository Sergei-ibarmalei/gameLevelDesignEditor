#include "Table/app.h"
#include "video.h"

int main(int, char**)
{
    App app;
    if (!app.initSdl(WINDOW_W, WINDOW_H))
        return 1;
    app.run();
    app.shutdown();
    return 0;
}
