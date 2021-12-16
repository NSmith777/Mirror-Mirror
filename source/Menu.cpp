#include "Menu.h"

//=============================================================================
// Menu::Menu
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:	[GfxDevice *]	Graphics device object
//              [FT_Library *]  Freetype instance
// 
// Return:      N/A
// 
//=============================================================================
Menu::Menu(GfxDevice* gfxDevice, FT_Library* pFt) {
    m_GfxDevice = gfxDevice;
    m_IsRunning = true;

    m_UnlitShader = new Shader(m_GfxDevice, "media/shaders/unlit_vs.cso", "media/shaders/unlit_ps.cso", sizeof(Constants));
    m_EnvMapShader = new Shader(m_GfxDevice, "media/shaders/envmap_vs.cso", "media/shaders/envmap_ps.cso", sizeof(Constants));
    m_TextShader = new Shader(m_GfxDevice, "media/shaders/text_vs.cso", "media/shaders/text_ps.cso", sizeof(Constants));
    m_ScreenShader = new Shader(m_GfxDevice, "media/shaders/screen_vs.cso", "media/shaders/screen_ps.cso", sizeof(Constants));

    m_TextShader->BlendEnable(true);
    m_TextShader->ZWriteEnable(false);

    m_UnlitShader->BlendEnable(true);
    m_UnlitShader->ZWriteEnable(false);

    m_LogoTex   = new Texture(m_GfxDevice, "media/objects/3DLogo/3DLogoTex.bmp");
    m_LogoMdl   = new Model(m_GfxDevice, "media/objects/3DLogo/3DLogo.mdl");
    m_Logo      = new GameObject(m_LogoMdl, m_LogoTex, m_EnvMapShader);

    m_Camera = new Camera(m_GfxDevice, m_ScreenShader);
    m_Camera->SetClearColor(0.1406f, 0.1836f, 0.2695f);

    m_Camera->GetTransform()->SetPosition({ 0.0f, 0.0f, -10.0f });

    m_ButtonTex     = new Texture(m_GfxDevice, "media/ui/button.bmp");
    m_PanelTex      = new Texture(m_GfxDevice, "media/ui/panel.bmp");
    m_LvlButtonTex  = new Texture(m_GfxDevice, "media/ui/lvlsel_button.bmp");

    m_StartBtn  = new Image(m_GfxDevice, m_ButtonTex, m_UnlitShader, { -150, -280 }, { 300, 75 });
    m_BackBtn   = new Image(m_GfxDevice, m_ButtonTex, m_UnlitShader, { -150, -250 }, { 300, 75 });
    m_Panel     = new Image(m_GfxDevice, m_PanelTex, m_UnlitShader, { -300, -160 }, { 600, 400 });

    m_Font = new Font(m_GfxDevice, pFt, "media/fonts/Roboto-Medium.ttf", 54);

    m_StartText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, -256 }, 0.65f);
    m_StartText->SetText("CLICK TO START");
    m_StartText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    m_BackText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, -228 }, 0.8f);
    m_BackText->SetText("BACK");
    m_BackText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    m_LvlSelText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, 256 }, 1.0f);
    m_LvlSelText->SetText("Level Select");
    m_LvlSelText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    char lvlnum_str[4];
    int lvlnum = 1;
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 6; x++) {
            XMFLOAT2 lvlbutton_pos(
                (float)(-270 + (x * 95)),
                (float)(150 - (y * 70))
            );

            m_LvlButtons[(y * 6) + x] = new Image(m_GfxDevice, m_LvlButtonTex, m_UnlitShader, lvlbutton_pos, { 64, 64 });

            XMFLOAT2 lvlnum_pos(
                lvlbutton_pos.x + 32,
                lvlbutton_pos.y + 18
            );

            sprintf(lvlnum_str, "%i", lvlnum);

            m_LvlBtnTexts[(y * 6) + x] = new Text(m_GfxDevice, m_Font, m_TextShader, lvlnum_pos, 0.75f);
            m_LvlBtnTexts[(y * 6) + x]->SetText(lvlnum_str);
            m_LvlBtnTexts[(y * 6) + x]->SetJustify(Text::TextAlign::ALIGN_CENTER);

            lvlnum++;
        }
    }

    m_MenuState = MenuState::STATE_TITLE;
    m_ReturnChoice = 0;

    m_MouseCoords = { 0, 0 };

    m_OscillateAmount = 0.0f;
}

//=============================================================================
// Menu::RunTitle
//=============================================================================
// 
// Description: Runs the menu title sequence.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Menu::RunTitle() {
    m_Camera->Use();

    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case WM_MOUSEMOVE:
            m_MouseCoords.x = GET_X_LPARAM(msg.lParam);
            m_MouseCoords.y = GET_Y_LPARAM(msg.lParam);
            break;
        case WM_LBUTTONDOWN:
            if (m_StartBtn->IsHovering(m_Camera, m_MouseCoords))
                m_MenuState = MenuState::STATE_LVLSEL;
            break;
        }
        DispatchMessageA(&msg);
    }

    m_Logo->GetTransform()->SetRotation({ cosf(m_OscillateAmount)/2, sinf(m_OscillateAmount), 0.0f });
    m_Logo->Render(m_Camera);

    m_OscillateAmount += 0.025f;

    m_StartBtn->Render(m_Camera);
    m_StartText->Render(m_Camera);

    m_GfxDevice->Present(m_Camera, 1);
}

//=============================================================================
// Menu::RunLvlSelect
//=============================================================================
// 
// Description: Runs the menu level select sequence.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Menu::RunLvlSelect() {
    m_Camera->Use();

    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case WM_MOUSEMOVE:
            m_MouseCoords.x = GET_X_LPARAM(msg.lParam);
            m_MouseCoords.y = GET_Y_LPARAM(msg.lParam);
            break;
        case WM_LBUTTONDOWN:
            if (m_BackBtn->IsHovering(m_Camera, m_MouseCoords))
                m_MenuState = MenuState::STATE_TITLE;

            for (int i = 0; i < 30; i++) {
                if (m_LvlButtons[i]->IsHovering(m_Camera, m_MouseCoords)) {
                    m_IsRunning = false;
                    m_ReturnChoice = i + 1;
                }
            }
            break;
        }
        DispatchMessageA(&msg);
    }

    m_LvlSelText->Render(m_Camera);

    m_Panel->Render(m_Camera);

    m_BackBtn->Render(m_Camera);
    m_BackText->Render(m_Camera);

    for (int i = 0; i < 30; i++) {
        m_LvlButtons[i]->Render(m_Camera);
        m_LvlBtnTexts[i]->Render(m_Camera);
    }

    m_GfxDevice->Present(m_Camera, 1);
}

//=============================================================================
// Menu::Update
//=============================================================================
// 
// Description: Runs the main menu loop.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Menu::Update() {
    while (m_IsRunning) {
        switch (m_MenuState) {
        case MenuState::STATE_TITLE:
            RunTitle();
            break;
        case MenuState::STATE_LVLSEL:
            RunLvlSelect();
            break;
        }
    }
}

//=============================================================================
// Menu::~Menu
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Menu::~Menu() {
    delete m_UnlitShader;
    delete m_EnvMapShader;
    delete m_TextShader;
    delete m_ScreenShader;

    delete m_LogoTex;
    delete m_LogoMdl;
    delete m_Logo;

    delete m_Camera;

    delete m_ButtonTex;
    delete m_PanelTex;
    delete m_LvlButtonTex;

    delete m_StartBtn;
    delete m_BackBtn;
    delete m_Panel;

    delete m_StartText;
    delete m_BackText;
    delete m_LvlSelText;

    for (int i = 0; i < 30; i++) {
        delete m_LvlButtons[i];
        delete m_LvlBtnTexts[i];
    }

    delete m_Font;
}
