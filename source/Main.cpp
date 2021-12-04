#include <windows.h>
#include <d3d11.h>

#include "Level.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        assert(false);

    while (true) {
        Level myLevel(&myGfxDevice, &ft);

        myLevel.Update();
    }

    FT_Done_FreeType(ft);

    return 0;
}
