#include "GameObject.h"

//=============================================================================
// GameObject::GameObject
//=============================================================================
// 
// Description: Constructor.
// 
// Parameters:  [Model *]   Mesh used to visually represent this GameObject on-screen
//              [Texture *] Texture to be used on the mesh
//              [Shader *]  Shader used to render the mesh to the screen
// 
// Returns:     N/A
// 
//=============================================================================
GameObject::GameObject(Model* mdl, Texture* tex, Shader* shdr) {
    SetModel(mdl);
    SetTexture(tex);
    SetShader(shdr);

    m_BoxCollision = nullptr;
    m_Transform = new Transform();

    ZeroMemory(&m_Constants, sizeof(m_Constants));
}

//=============================================================================
// GameObject::AddBoxCollider
//=============================================================================
// 
// Description: Instantiates a box collider bound to this GameObject and its transform.
// 
// Parameters:	[XMFLOAT3]  Box dimensions, relative to this GameObject transform's center
// 
// Returns:     N/A
// 
//=============================================================================
void GameObject::AddBoxCollider(XMFLOAT3 size) {
    m_BoxCollision = new BoxCollision(m_Transform, size);
}

//=============================================================================
// GameObject::Render
//=============================================================================
// 
// Description: Draws this game object to the screen, relative to a camera.
// 
// Parameters:	[Camera *]  Camera to draw relative to
// 
// Returns:     N/A
// 
//=============================================================================
void GameObject::Render(Camera* cam) {
    m_Constants.MVP = m_Transform->GetModelMatrix() * cam->GetViewMatrix() * cam->GetProjMatrix();
    m_Constants.MV = m_Transform->GetModelMatrix() * cam->GetViewMatrix();
    m_Constants.M = m_Transform->GetModelMatrix();
    m_Constants._World2Object = XMMatrixInverse(NULL, m_Constants.M);
    m_Constants._WorldSpaceCameraPos = cam->GetTransform()->GetPosition();

    m_Shader->SetConstants(&m_Constants);

    m_Shader->Use();
    m_Texture->Use();
    m_Model->Draw();
}

//=============================================================================
// GameObject::~GameObject
//=============================================================================
// 
// Description: Destructor.
// 
// Parameters:  N/A
// 
// Returns:     N/A
// 
//=============================================================================
GameObject::~GameObject() {
    if (m_BoxCollision != nullptr)
        delete m_BoxCollision;

    delete m_Transform;
}
