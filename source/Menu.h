#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "Framework/GfxDevice.h"
#include "Framework/Font.h"
#include "Framework/Text.h"
#include "Framework/Texture.h"
#include "Framework/Image.h"
#include "Framework/Math.h"

#include "GameObject.h"

using namespace Math;

class Menu {
public:
    Menu(GfxDevice* gfxDevice, FT_Library* pFt);
    ~Menu();

    void Update();

    int GetReturnChoice() { return m_ReturnChoice; }

private:
    void RunTitle();
    void RunLvlSelect();

    GfxDevice* m_GfxDevice;
    bool m_IsRunning;

    Shader* m_UnlitShader, *m_EnvMapShader, *m_TextShader, *m_ScreenShader;

    Texture* m_LogoTex;
    Model* m_LogoMdl;
    GameObject* m_Logo;

    Camera* m_Camera;

    Texture* m_ButtonTex, *m_PanelTex, *m_LvlButtonTex;
    Image* m_StartBtn, *m_BackBtn, *m_Panel, *m_LvlButtons[30];
    Text* m_StartText, *m_BackText, *m_LvlSelText, *m_LvlBtnTexts[30];

    Font* m_Font;

    enum class MenuState {
        STATE_TITLE,
        STATE_LVLSEL,
    };

    MenuState m_MenuState;
    int m_ReturnChoice;

    XMINT2 m_MouseCoords;

    float m_OscillateAmount;
};
