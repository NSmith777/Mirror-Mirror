//==============================================================================
// File: Main.cpp
// 
// Description: This file contains the entry point to the game.
// 
//==============================================================================

#include <windows.h>
#include <d3d11.h>

#include "Level.h"

//=============================================================================
// WinMain
//=============================================================================
// 
// Description: Main Windows entry point - contains the basic game loop.
// 
// Parameters:  Standard win32 parameters
// 
// Returns:     Exit code (int)
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
    while (true) {
        Level myLevel(&myGfxDevice, &ft);

        myLevel.Update();
    }

    //
    // Shutdown Freetype
    //
    FT_Done_FreeType(ft);

    return 0;
}
