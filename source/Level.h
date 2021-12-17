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

class Level {
public:
    Level(GfxDevice* myGfxDevice, FT_Library *pFt, int level_num);
    ~Level();

    void Update();

    enum class LevelReturnChoice {
        LVLRET_MAINMENU,
        LVLRET_RETRY,
        LVLRET_NEXTLVL,
    };

    LevelReturnChoice GetReturnChoice() { return m_ReturnChoice; }

private:
    void LoadLayoutFromFile(int level_num);

    void RunGame();
    void RunGameOver();
    void RunComplete();

    GfxDevice* m_GfxDevice;
    bool m_IsRunning;

    Shader *m_ScreenShader, *m_TextShader, *m_UnlitShader, *m_DefaultShader;

    Camera* m_Camera;

    Font *m_Font;

    Text *m_TimeText, *m_MovesCntText;

    Texture *m_PlayerTex;
    Model* m_PlayerMdl;
    GameObject* m_Player;

    Texture* m_DrawLineGuideTex;
    Model* m_DrawLineGuideMdl;
    GameObject* m_MirrorLine, *m_ReflectLine;
    
    Texture *m_DrawLineTargetTex, *m_DrawLineTargetNGTex;
    Model* m_DrawLineTargetMdl;
    GameObject* m_DrawLineTarget;

    Texture *m_GoalTex;
    Model* m_GoalUnpressedMdl, *m_GoalPressedMdl;
    GameObject* m_Goal;

    Texture *m_GroundTex;
    Model *m_GroundMdl;
    std::vector<GameObject*> m_Grounds;

    Texture* m_WallTex;
    Model* m_WallMdl;
    std::vector<GameObject*> m_Walls;

    Text* m_MenuText;
    Text* m_LeftOptionText, * m_RightOptionText;

    Texture* m_PanelTex;
    Image* m_MenuPanel;

    Texture* m_ButtonTex;
    Image* m_LeftOption, *m_RightOption;

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
    LevelReturnChoice m_ReturnChoice;

    XMFLOAT3 m_CamOffset;

    XMINT2 m_MouseCoords;
    XMFLOAT3 m_MirrorStart, m_MirrorTarget;

    float m_StartTime;
    float m_ElapsedTime;

    int m_MovesCount;

    bool m_IsMouseHeld;
    bool m_IsReflectLineDrawn;
    bool m_CanMove;
};
