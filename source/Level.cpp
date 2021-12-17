#include "Level.h"

//=============================================================================
// Level::Level
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
Level::Level(GfxDevice *myGfxDevice, FT_Library* pFt, int level_num) {
    m_GfxDevice = myGfxDevice;
    m_IsRunning = true;

    // Instantiate shaders
    m_ScreenShader  = new Shader(m_GfxDevice, "media/shaders/screen_vs.cso", "media/shaders/screen_ps.cso", sizeof(Constants));
    m_TextShader    = new Shader(m_GfxDevice, "media/shaders/text_vs.cso", "media/shaders/text_ps.cso", sizeof(Constants));
    m_UnlitShader   = new Shader(m_GfxDevice, "media/shaders/unlit_vs.cso", "media/shaders/unlit_ps.cso", sizeof(Constants));
    m_DefaultShader = new Shader(m_GfxDevice, "media/shaders/default_vs.cso", "media/shaders/default_ps.cso", sizeof(Constants));

    m_TextShader->ZWriteEnable(false);
    m_TextShader->BlendEnable(true);

    m_UnlitShader->ZWriteEnable(false);
    m_UnlitShader->BlendEnable(true);

    m_DefaultShader->BlendEnable(true);

    // Create main camera
    m_Camera = new Camera(m_GfxDevice, m_ScreenShader);
    m_Camera->SetClearColor(0.25f, 0.25f, 0.25f);

    m_Camera->GetTransform()->SetPosition({ 0.0f, 20.0f, -20.0f });
    m_Camera->GetTransform()->SetRotation({ XMConvertToRadians(60.0f), 0.0f, 0.0f });

    m_CamOffset = { 0.0f, 30.0f, -18.0f };

    // Instantiate font
    m_Font = new Font(m_GfxDevice, pFt, "media/fonts/Roboto-Medium.ttf", 54);

    // Create timer text object
    m_TimeText = new Text(m_GfxDevice, m_Font, m_TextShader, { -610, -215 }, 1.0f);
    m_TimeText->SetText("Time: 00:00:00");

    m_StartTime = m_GfxDevice->GetTime();
    m_ElapsedTime = 0.0f;

    // Create moves count text object
    m_MovesCntText = new Text(m_GfxDevice, m_Font, m_TextShader, { -610, -290 }, 1.2f);
    m_MovesCntText->SetText("Moves: 0");

    m_MovesCount = 0;

    // Create the player
    m_PlayerTex = new Texture(m_GfxDevice, "media/objects/Player/TestTexture.bmp");
    m_PlayerMdl = new Model(m_GfxDevice, "media/objects/Player/TestPlayer.mdl");
    m_Player    = new GameObject(m_PlayerMdl, m_PlayerTex, m_DefaultShader);

    // Create the mirror guidelines
    m_DrawLineGuideTex = new Texture(m_GfxDevice, "media/objects/DrawLine/DrawLine_Guide_Dif.bmp");
    m_DrawLineGuideMdl = new Model(m_GfxDevice, "media/objects/DrawLine/TestDrawLine_Guide.mdl");
    m_MirrorLine       = new GameObject(m_DrawLineGuideMdl, m_DrawLineGuideTex, m_DefaultShader);
    m_ReflectLine      = new GameObject(m_DrawLineGuideMdl, m_DrawLineGuideTex, m_DefaultShader);

    // Create mirror target crosshair object
    m_DrawLineTargetTex   = new Texture(m_GfxDevice, "media/objects/DrawLine/DrawLine_Target_Dif.bmp");
    m_DrawLineTargetNGTex = new Texture(m_GfxDevice, "media/objects/DrawLine/DrawLine_Target_NG_Dif.bmp");
    m_DrawLineTargetMdl   = new Model(m_GfxDevice, "media/objects/DrawLine/TestDrawLine_Target.mdl");
    m_DrawLineTarget      = new GameObject(m_DrawLineTargetMdl, m_DrawLineTargetTex, m_DefaultShader);

    m_DrawLineTarget->GetTransform()->SetScale({ 1.75f, 1.75f, 1.75f });

    // Create goal switch object
    m_GoalTex          = new Texture(m_GfxDevice, "media/objects/Goal/Goal_Dif.bmp");
    m_GoalUnpressedMdl = new Model(m_GfxDevice, "media/objects/Goal/GoalUnpressed.mdl");
    m_GoalPressedMdl   = new Model(m_GfxDevice, "media/objects/Goal/GoalPressed.mdl");
    m_Goal             = new GameObject(m_GoalUnpressedMdl, m_GoalTex, m_DefaultShader);

    m_Goal->AddBoxCollider({ 2, 2, 2 });

    // Setup ground object components
    m_GroundTex = new Texture(m_GfxDevice, "media/objects/CobbleGroundNormal/CobbleGroundNormal_Dif.bmp");
    m_GroundMdl = new Model(m_GfxDevice, "media/objects/CobbleGroundNormal/CobbleGroundNormal.mdl");

    // Setup wall object components
    m_WallTex = new Texture(m_GfxDevice, "media/objects/StoneWall/StoneWall_Dif.bmp");
    m_WallMdl = new Model(m_GfxDevice, "media/objects/StoneWall/StoneWall.mdl");

    // Create menu heading text object
    m_MenuText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, 0 }, 1.25f);

    m_MenuText->SetPosition({ 0, 80 });
    m_MenuText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    // Create menu left option text object
    m_LeftOptionText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, 0 }, 0.8f);

    m_LeftOptionText->SetPosition({ -150, -122 });
    m_LeftOptionText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    // Create menu right option text object
    m_RightOptionText = new Text(m_GfxDevice, m_Font, m_TextShader, { 0, 0 }, 0.8f);

    m_RightOptionText->SetPosition({ 150, -122 });
    m_RightOptionText->SetJustify(Text::TextAlign::ALIGN_CENTER);

    // Create menu panel object
    m_PanelTex  = new Texture(m_GfxDevice, "media/ui/panel.bmp");
    m_MenuPanel = new Image(m_GfxDevice, m_PanelTex, m_UnlitShader, { -300, -160 }, { 600, 400 });

    // Create menu button objects
    m_ButtonTex   = new Texture(m_GfxDevice, "media/ui/button.bmp");
    m_LeftOption  = new Image(m_GfxDevice, m_ButtonTex, m_UnlitShader, { -280, -140 }, { 256, 64 });
    m_RightOption = new Image(m_GfxDevice, m_ButtonTex, m_UnlitShader, { 20, -140 }, { 256, 64 });

    // Initialize our level properties
    m_LevelState = LevelState::STATE_MAIN;
    m_ReturnChoice = LevelReturnChoice::LVLRET_MAINMENU;

    m_MouseCoords = { 0, 0 };
    m_MirrorStart = { 0, 0, 0 };
    m_MirrorTarget = { 0, 0, 0 };

    m_IsMouseHeld = false;
    m_IsReflectLineDrawn = false;
    m_CanMove = false;

    // After we've set our member variables, we can finally setup the level layout
    LoadLayoutFromFile(level_num);
}

//=============================================================================
// Level::LoadLayoutFromFile
//=============================================================================
// 
// Description: Builds the level layout from an interpreted text file.
// 
// Parameters:	[int]   Level number
// 
// Return:      N/A
// 
//=============================================================================
void Level::LoadLayoutFromFile(int level_num) {
    char lvlfile_path[512];
    sprintf(lvlfile_path, "media/levels/lvl%i.txt", level_num);

    FILE* lvlfile = fopen(lvlfile_path, "r");

    if (!lvlfile)
        assert(false);

    char lvlfile_buf[512] = { '\0' };

    while (fscanf(lvlfile, "%s\n", lvlfile_buf) == 1) {
        if (strcmp(lvlfile_buf, "LAYER") == 0) {
            int width = 0, height = 0;
            if (fscanf(lvlfile, "%i %i\n", &width, &height) != 2)
                assert(false);

            for (int z = 0; z < height; z++) {
                for (int x = 0; x < width; x++) {
                    LevelObjectId gameobject_id;
                    if (fscanf(lvlfile, "%i,", &gameobject_id) != 1)
                        assert(false);

                    XMFLOAT3 obj_pos(x * 8.0f, 0.0f, z * -8.0f);

                    switch (gameobject_id) {
                    case LevelObjectId::ID_PLAYER:
                        m_Player->GetTransform()->SetPosition(obj_pos);
                        break;
                    case LevelObjectId::ID_GOAL:
                        m_Goal->GetTransform()->SetPosition(obj_pos);
                        break;
                    case LevelObjectId::ID_GROUND:
                        m_Grounds.push_back(new GameObject(m_GroundMdl, m_GroundTex, m_DefaultShader));

                        m_Grounds[m_Grounds.size() - 1]->AddBoxCollider({ 4, 4, 4 });
                        m_Grounds[m_Grounds.size() - 1]->GetTransform()->SetPosition(obj_pos);
                        break;
                    case LevelObjectId::ID_WALL:
                        m_Walls.push_back(new GameObject(m_WallMdl, m_WallTex, m_DefaultShader));

                        m_Walls[m_Walls.size() - 1]->AddBoxCollider({ 4, 8, 4 });
                        m_Walls[m_Walls.size() - 1]->GetTransform()->SetPosition(obj_pos);
                        break;
                    default:
                        break;
                    }
                }

                if (fscanf(lvlfile, "\n") != 0)
                    assert(false);
            }
        }
    }

    fclose(lvlfile);
}

static inline bool ccw(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C) {
    return (C.z - A.z) * (B.x - A.x) > (B.z - A.z) * (C.x - A.x);
}

static inline bool is_xz_line_intersect(XMFLOAT3 A, XMFLOAT3 B, XMFLOAT3 C, XMFLOAT3 D) {
    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}

//=============================================================================
// Level::RunGame
//=============================================================================
// 
// Description: Runs the main level game sequence.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Level::RunGame() {
    XMFLOAT3 player_pos = m_Player->GetTransform()->GetPosition();
    Transform* mirror_line_transform = m_MirrorLine->GetTransform();
    Transform* reflect_line_transform = m_ReflectLine->GetTransform();

    ////////// Camera //////////

    m_Camera->Use();

    XMFLOAT3 cam_lerp = XMFLOAT3_Lerp(m_Camera->GetTransform()->GetPosition(), player_pos + m_CamOffset, 0.1f);

    m_Camera->GetTransform()->SetPosition(cam_lerp);

    XMFLOAT3 cam_pos = m_Camera->GetTransform()->GetPosition();
    XMFLOAT3 cam_ray_origin = m_Camera->ScreenToWorldPoint(m_MouseCoords);

    // Raycast camera ray to the plane y=0
    XMFLOAT3 cam_ray_hit = -cam_pos.y * (cam_ray_origin - cam_pos) / (cam_ray_origin.y - cam_pos.y) + cam_pos;

    ////////// Input //////////

    MSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
        switch (msg.message) {
        case WM_MOUSEMOVE:
            m_MouseCoords.x = GET_X_LPARAM(msg.lParam);
            m_MouseCoords.y = GET_Y_LPARAM(msg.lParam);
            break;
        case WM_LBUTTONDOWN:
            switch(m_LevelState) {
            case LevelState::STATE_MAIN:
                mirror_line_transform->SetPosition(cam_ray_hit);
                reflect_line_transform->SetPosition(player_pos);

                m_MirrorStart = cam_ray_hit;

                m_IsMouseHeld = true;
                break;
            case LevelState::STATE_GAME_OVER:
                if (m_LeftOption->IsHovering(m_Camera, m_MouseCoords)) {
                    m_IsRunning = false;
                    m_ReturnChoice = LevelReturnChoice::LVLRET_MAINMENU;
                }
                else if (m_RightOption->IsHovering(m_Camera, m_MouseCoords)) {
                    m_IsRunning = false;
                    m_ReturnChoice = LevelReturnChoice::LVLRET_RETRY;
                }
                break;
            case LevelState::STATE_COMPLETED:
                if (m_LeftOption->IsHovering(m_Camera, m_MouseCoords)) {
                    m_IsRunning = false;
                    m_ReturnChoice = LevelReturnChoice::LVLRET_RETRY;
                }
                else if (m_RightOption->IsHovering(m_Camera, m_MouseCoords)) {
                    m_IsRunning = false;
                    m_ReturnChoice = LevelReturnChoice::LVLRET_NEXTLVL;
                }
                break;
            }
            break;
        case WM_LBUTTONUP:
            // Check that we're actually allowed to move,
            // as well as the mirror guide not hitting any walls!
            if (m_LevelState == LevelState::STATE_MAIN && m_IsReflectLineDrawn && m_CanMove) {
                // Move the player to the new mirror target!
                m_Player->GetTransform()->SetPosition(m_MirrorTarget);

                XMFLOAT3 hit;

                // Collision step for the goal switch,
                // set the level as complete if the player hits it.
                bool is_goal_hit = m_Goal->GetBoxCollision()->Ray_Intersect(
                    m_Player->GetTransform()->GetPosition(),
                    m_Player->GetTransform()->GetPosition() + XMFLOAT3(0, 4, 0),
                    hit
                    );

                if (is_goal_hit)
                    m_LevelState = LevelState::STATE_COMPLETED;

                // Do collision step for all ground platforms against the player.
                bool is_landed = false;
                for (unsigned int i = 0; i < m_Grounds.size(); i++) {
                    bool is_hit = m_Grounds[i]->GetBoxCollision()->Ray_Intersect(
                        m_Player->GetTransform()->GetPosition(),
                        m_Player->GetTransform()->GetPosition() + XMFLOAT3(0, 4, 0),
                        hit
                    );

                    if (is_hit)
                        is_landed = true;
                }

                // If the player didn't land onto any ground, then they probably fell off...
                if(!is_landed)
                    m_LevelState = LevelState::STATE_GAME_OVER;

                // Update our number of moves made.
                m_MovesCount++;

                char moves_count_str[16];
                sprintf(moves_count_str, "Moves: %i", m_MovesCount);
                m_MovesCntText->SetText(moves_count_str);
            }

            m_IsMouseHeld = false;
            break;
        }
        DispatchMessageA(&msg);
    }

    // Update our timer
    if (m_LevelState == LevelState::STATE_MAIN) {
        m_ElapsedTime = m_GfxDevice->GetTime() - m_StartTime;

        char time_str[32];
        sprintf(time_str, "Time: %02i:%02i:%02i",
            (int)m_ElapsedTime / 60,
            (int)m_ElapsedTime % 60,
            (int)(m_ElapsedTime * 100) % 100
        );

        m_TimeText->SetText(time_str);
    }

    // Calculate the mirror draw lines
    if (m_IsMouseHeld) {
        float m = (cam_ray_hit.z - m_MirrorStart.z) / (cam_ray_hit.x - m_MirrorStart.x);
        float c = (cam_ray_hit.x * m_MirrorStart.z - m_MirrorStart.x * cam_ray_hit.z) / (cam_ray_hit.x - m_MirrorStart.x);
        float d = (player_pos.x + (player_pos.z - c) * m) / (1 + (m * m));

        m_MirrorTarget.x = 2 * d - player_pos.x;
        m_MirrorTarget.z = 2 * d * m - player_pos.z + 2 * c;

        m_IsReflectLineDrawn = is_xz_line_intersect(
            mirror_line_transform->GetPosition(),
            cam_ray_hit,
            reflect_line_transform->GetPosition(),
            m_MirrorTarget
        );

        // Align the mirror line to stretch between the two points drawn by the mouse.
        XMFLOAT3 mirror_line_delta = cam_ray_hit - mirror_line_transform->GetPosition();

        mirror_line_transform->SetRotation({ 0, atan2f(mirror_line_delta.x, mirror_line_delta.z), 0 });
        mirror_line_transform->SetScale({ 0.4f, 0.4f, XMFLOAT3_Distance(mirror_line_transform->GetPosition(), cam_ray_hit) });

        // Align the reflection guide to stretch between the player and target point.
        XMFLOAT3 reflect_line_delta = m_MirrorTarget - reflect_line_transform->GetPosition();

        reflect_line_transform->SetRotation({ 0, atan2f(reflect_line_delta.x, reflect_line_delta.z), 0 });
        reflect_line_transform->SetScale({ 0.4f, 0.4f, XMFLOAT3_Distance(reflect_line_transform->GetPosition(), m_MirrorTarget) });

        // Perform collision step for all walls in the scene.
        // We build an array of every wall the mirror guide hits,
        // So we can sort and grab the closest wall hit later on.
        std::vector<XMFLOAT3> hits;

        for (unsigned int i = 0; i < m_Walls.size(); i++) {
            XMFLOAT3 cur_hit;

            bool is_hit = m_Walls[i]->GetBoxCollision()->Ray_Intersect(
                reflect_line_transform->GetPosition(),
                m_MirrorTarget,
                cur_hit
            );

            if (is_hit)
                hits.push_back(cur_hit);
        }

        // If the mirror guide goes through multiple walls,
        // perform a sort (by distance) for the closest wall hit, then use its hit point.
        XMFLOAT3 out_hit;

        if (hits.size() > 0) {
            float distance = XMFLOAT3_Distance(reflect_line_transform->GetPosition(), hits[0]);
            int closest_hit = 0;

            for (unsigned int i = 0; i < hits.size(); i++) {
                if (XMFLOAT3_Distance(reflect_line_transform->GetPosition(), hits[i]) < distance)
                    closest_hit = i;
            }

            out_hit = hits[closest_hit];
            m_CanMove = false;
        }
        else {
            m_CanMove = true;
        }

        if (m_CanMove) {
            m_DrawLineTarget->GetTransform()->SetPosition(m_MirrorTarget);
            m_DrawLineTarget->GetTransform()->Rotate({ 0.0f, XMConvertToRadians(1.5f), 0.0f });
        }
        else {
            m_DrawLineTarget->GetTransform()->SetPosition(out_hit);
            m_DrawLineTarget->GetTransform()->SetRotation({ 0.0f, 0.0f, 0.0f });
        }
    }

    ////////// RENDERING //////////

    m_Player->Render(m_Camera);

    m_Goal->Render(m_Camera);

    for (unsigned int i = 0; i < m_Grounds.size(); i++)
        m_Grounds[i]->Render(m_Camera);

    for (unsigned int i = 0; i < m_Walls.size(); i++)
        m_Walls[i]->Render(m_Camera);

    if (m_IsMouseHeld) {
        // Disable depth write for all lines
        m_DefaultShader->ZWriteEnable(false);

        m_MirrorLine->Render(m_Camera);

        if (m_IsReflectLineDrawn) {
            m_ReflectLine->Render(m_Camera);

            if (m_CanMove)
                m_DrawLineTarget->SetTexture(m_DrawLineTargetTex);
            else
                m_DrawLineTarget->SetTexture(m_DrawLineTargetNGTex);

            m_DrawLineTarget->Render(m_Camera);
        }

        // Re-enable depth write
        m_DefaultShader->ZWriteEnable(true);
    }

    m_TimeText->Render(m_Camera);

    m_MovesCntText->Render(m_Camera);
}

//=============================================================================
// Level::RunGameOver
//=============================================================================
// 
// Description: Displays the game over menu screen.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Level::RunGameOver() {
    m_MenuText->SetText("Game Over!");
    m_LeftOptionText->SetText("QUIT");
    m_RightOptionText->SetText("RETRY");

    m_MenuPanel->Render(m_Camera);
    m_LeftOption->Render(m_Camera);
    m_RightOption->Render(m_Camera);

    m_MenuText->Render(m_Camera);
    m_LeftOptionText->Render(m_Camera);
    m_RightOptionText->Render(m_Camera);
}

//=============================================================================
// Level::RunComplete
//=============================================================================
// 
// Description: Displays the level complete menu screen.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Level::RunComplete() {
    m_MenuText->SetText("Level Completed!");
    m_LeftOptionText->SetText("RETRY");
    m_RightOptionText->SetText("NEXT");

    m_MenuPanel->Render(m_Camera);
    m_LeftOption->Render(m_Camera);
    m_RightOption->Render(m_Camera);

    m_MenuText->Render(m_Camera);
    m_LeftOptionText->Render(m_Camera);
    m_RightOptionText->Render(m_Camera);
}

//=============================================================================
// Level::Update
//=============================================================================
// 
// Description: Runs the main level loop.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
void Level::Update() {
    while (m_IsRunning) {
        RunGame();

        switch (m_LevelState) {
        case LevelState::STATE_PAUSED:
            break;
        case LevelState::STATE_GAME_OVER:
            RunGameOver();
            break;
        case LevelState::STATE_COMPLETED:
            RunComplete();
            break;
        }

        m_GfxDevice->Present(m_Camera, 1);
    }
}

//=============================================================================
// Level::~Level
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:	N/A
// 
// Return:      N/A
// 
//=============================================================================
Level::~Level() {
    delete m_ScreenShader;
    delete m_TextShader;
    delete m_UnlitShader;
    delete m_DefaultShader;

    delete m_Font;

    delete m_TimeText;
    delete m_MovesCntText;

    delete m_PlayerTex;
    delete m_DrawLineGuideTex;
    delete m_DrawLineTargetTex;
    delete m_DrawLineTargetNGTex;
    delete m_GoalTex;
    delete m_GroundTex;
    delete m_WallTex;

    delete m_PlayerMdl;
    delete m_DrawLineGuideMdl;
    delete m_DrawLineTargetMdl;
    delete m_GoalUnpressedMdl;
    delete m_GoalPressedMdl;
    delete m_GroundMdl;
    delete m_WallMdl;

    delete m_Player;
    delete m_MirrorLine;
    delete m_ReflectLine;
    delete m_DrawLineTarget;
    delete m_Goal;

    for (unsigned int i = 0; i < m_Grounds.size(); i++)
        delete m_Grounds[i];

    for (unsigned int i = 0; i < m_Walls.size(); i++)
        delete m_Walls[i];

    delete m_MenuText;
    delete m_LeftOptionText;
    delete m_RightOptionText;

    delete m_ButtonTex;
    delete m_PanelTex;

    delete m_MenuPanel;
    delete m_LeftOption;
    delete m_RightOption;

    delete m_Camera;
}
