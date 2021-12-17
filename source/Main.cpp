#include <windows.h>
#include <d3d11.h>

#include "Menu.h"
#include "Level.h"

//=============================================================================
// WinMain
//=============================================================================
// 
// Description: Main Windows entry point - contains the basic game loop.
// 
// Parameters:  Standard win32 parameters
// 
// Returns:     [int]   Exit code
// 
//=============================================================================
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    //
    // Create our graphics device object, which will initialize the graphics backend and game window.
    //
    GfxDevice myGfxDevice;

    //
    // Initialize Freetype
    //
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        assert(false);

    //
    // Run our main game loop
    //
    bool run_menu = true;
    int level_num = 1;
    while (true) {
        if (run_menu) {
            Menu myMenu(&myGfxDevice, &ft);
            myMenu.Update();

            level_num = myMenu.GetReturnChoice();
        }

        Level myLevel(&myGfxDevice, &ft, level_num);
        myLevel.Update();

        switch (myLevel.GetReturnChoice()) {
        case Level::LevelReturnChoice::LVLRET_MAINMENU:
            run_menu = true;
            break;
        case Level::LevelReturnChoice::LVLRET_RETRY:
            run_menu = false;
            break;
        case Level::LevelReturnChoice::LVLRET_NEXTLVL:
            level_num++;
            run_menu = level_num > 30 ? true : false;
            break;
        }
    }

    //
    // Shutdown Freetype
    //
    FT_Done_FreeType(ft);

    return 0;
}
