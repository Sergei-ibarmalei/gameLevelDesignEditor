#include "Table/app.h"
#include "video.h"

int main(int, char**)
{
    SDL_version compiled;
    SDL_VERSION(&compiled);
    SDL_version linked;
    SDL_GetVersion(&linked);

    SDL_Log("SDL compiled: %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
    SDL_Log("SDL linked:   %d.%d.%d", linked.major, linked.minor, linked.patch);
    App app;
    if (!app.initSdl(WINDOW_W, WINDOW_H))
        return 1;
    app.run();
    app.shutdown();
    return 0;
}
