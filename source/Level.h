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
    Level(GfxDevice* myGfxDevice, FT_Library *pFt, int level_num);
    ~Level();

    void Update();

    int GetReturnChoice() { return m_ReturnChoice; }

private:
    void RunGame();
    void RunGameOver();
    void RunComplete();

    GfxDevice* m_GfxDevice;
    bool m_IsRunning;

    Shader *m_ScreenShader, *m_TextShader, *m_UnlitShader, *m_DefaultShader;

    Font *m_Font;

    Text *m_TimeText, *m_MovesCntText;

    Texture *m_PlayerTex;
    Texture *m_DrawLineGuideTex, *m_DrawLineTargetTex, *m_DrawLineTargetNGTex;
    Texture *m_GoalTex;
    Texture *m_GroundTex, *m_WallTex;

    Model *m_PlayerMdl;
    Model *m_DrawLineGuideMdl, *m_DrawLineTargetMdl;
    Model *m_GoalUnpressedMdl, *m_GoalPressedMdl;
    Model *m_GroundMdl;
    Model *m_WallMdl;

    GameObject* m_Player;
    GameObject* m_MirrorLine, *m_ReflectLine, *m_DrawLineTarget;
    GameObject* m_Goal;

    std::vector<GameObject*> m_Grounds;
    std::vector<GameObject*> m_Walls;

    Text* m_MenuText, *m_LeftOptionText, *m_RightOptionText;
    Texture* m_ButtonTex, * m_PanelTex;
    Image* m_MenuPanel, *m_LeftOption, *m_RightOption;

    Camera *m_Camera;

    XMFLOAT3 m_CamOffset;

    XMINT2 m_MouseCoords;
    XMFLOAT3 m_MirrorStart, m_MirrorTarget;

    enum class LevelObjectId {
        ID_PLAYER = 1,
        ID_GOAL,
        ID_GROUND,
        ID_WALL,
    };

    enum class LevelState {
        STATE_MAIN,
        STATE_PAUSED,
        STATE_GAME_OVER,
        STATE_COMPLETED,
    };

    LevelState m_LevelState;
    int m_ReturnChoice;

    float m_StartTime;
    float m_ElapsedTime;

    int m_MovesCount;

    bool m_IsMouseHeld;
    bool m_IsReflectLineDrawn;
    bool m_CanMove;
};
