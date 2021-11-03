#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "GfxDevice.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"

using namespace DirectX;

typedef struct {
    XMMATRIX MVP;
} Constants;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    GfxDevice myGfxDevice;

    Shader myScreenShader(&myGfxDevice, "../resources/shaders/screen_vs.cso", "../resources/shaders/screen_ps.cso", 0);

    Camera myCamera(&myGfxDevice, &myScreenShader);
    myCamera.GetTransform()->Translate(0.0f, 0.0f, -4.0f);

    Shader myShader(&myGfxDevice, "../resources/shaders/default_vs.cso", "../resources/shaders/default_ps.cso", sizeof(Constants));
    Texture myTexture(&myGfxDevice);

    Model myModel1(&myGfxDevice, "../resources/models/test.mdl");
    Transform myTransform1;
    myTransform1.Translate(-2.0f, 0.0f, 0.0f);

    Model myModel2(&myGfxDevice, "../resources/models/test.mdl");
    Transform myTransform2;
    myTransform2.Translate(2.0f, 0.0f, 0.0f);

    while (true) {
        MSG msg;

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            switch (msg.message) {
            case WM_KEYDOWN:
                return 0;
                break;
            }
            DispatchMessageA(&msg);
        }

        myTransform1.Rotate(0.005f, 0.009f, 0.001f);

        myCamera.Use();
        
        ////////// Model 1 //////////

        Constants constants1;
        constants1.MVP = myTransform1.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants1);

        myShader.Use();
        myTexture.Use();
        myModel1.Draw();

        ////////// Model 2 //////////

        Constants constants2;
        constants2.MVP = myTransform2.GetModelMatrix() * myCamera.GetViewMatrix() * myCamera.GetProjMatrix();

        myShader.SetConstants(&constants2);

        myShader.Use();
        myTexture.Use();
        myModel2.Draw();

        myGfxDevice.Present(&myCamera, 1);
    }
}
