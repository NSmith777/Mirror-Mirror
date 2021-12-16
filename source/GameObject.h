#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "Framework/GfxDevice.h"
#include "Framework/Texture.h"
#include "Framework/Model.h"
#include "Framework/Collision.h"

class GameObject {
public:
    GameObject(Model *mdl, Texture* tex, Shader *shdr);
    ~GameObject();

    void SetModel(Model* mdl) { m_Model = mdl; }
    void SetTexture(Texture* tex) { m_Texture = tex; }
    void SetShader(Shader* shdr) { m_Shader = shdr; }

    void AddBoxCollider(XMFLOAT3 size);

    BoxCollision* GetBoxCollision() { return m_BoxCollision; }
    Transform* GetTransform() { return m_Transform; }

    // Draw this object relative to a Camera's view and projection
    void Render(Camera *cam);

    // Supress heap alignment warnings
    void* operator new(size_t i) { return _mm_malloc(i, 16); }
    void operator delete(void* p) { _mm_free(p); }

private:
    Shader* m_Shader;
    Texture* m_Texture;
    Model* m_Model;
    BoxCollision* m_BoxCollision;
    Transform *m_Transform;

    Constants m_Constants;
};
