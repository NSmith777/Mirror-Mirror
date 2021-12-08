//==============================================================================
// File: Main.cpp
// 
// Description: Declares the Level class.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "Framework/GfxDevice.h"
#include "Framework/Font.h"
#include "Framework/Text.h"
#include "Framework/Texture.h"
#include "Framework/Image.h"
#include "Framework/Model.h"
#include "Framework/Math.h"

using namespace Math;

#include "GameObject.h"

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class Level {
public:
	Level(GfxDevice* myGfxDevice, FT_Library *pFt);
	~Level();

	void Update();

private:
	GfxDevice* m_GfxDevice;
	bool is_running;

    Shader *myScreenShader;
    Shader *myTextShader;
    Shader *myShader;

    Font *myFont;

    Text *myText1;
    Text *myText2;

    Texture *PlayerTex;
    Texture *DrawLineGuideTex;
    Texture *DrawLineTargetTex;
    Texture *DrawLineTargetNGTex;
    Texture *GroundTex;
    Texture *WallTex;

    Image *myImage;

    Model *PlayerMdl;
    Model *DrawLineGuideMdl;
    Model *DrawLineTargetMdl;
    Model *GroundMdl;
    Model *WallMdl;

    GameObject *Player;
    GameObject *MirrorLine;
    GameObject *ReflectLine;
    GameObject *DrawLineTarget;

    std::vector<GameObject*> Grounds;
    std::vector<GameObject*> Walls;

    Camera *myCamera;

    XMINT2 mouse_coords;
    XMFLOAT3 mirror_start;
    XMFLOAT3 mirror_target;

    bool is_mouse_held;
    bool is_reflect_line_drawn;
    bool can_move;
};
