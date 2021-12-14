//==============================================================================
// File: GameObject.cpp
// 
// Description: Implements the GameObject class.
// 
//==============================================================================

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

    boxCollision = nullptr;
    transform = new Transform();

    ZeroMemory(&constants, sizeof(constants));
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
    boxCollision = new BoxCollision(transform, size);
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
    constants.MVP = transform->GetModelMatrix() * cam->GetViewMatrix() * cam->GetProjMatrix();
    constants.MV = transform->GetModelMatrix() * cam->GetViewMatrix();
    constants.M = transform->GetModelMatrix();
    constants._World2Object = XMMatrixInverse(NULL, constants.M);
    constants._WorldSpaceCameraPos = cam->GetTransform()->GetPosition();

    shader->SetConstants(&constants);

    shader->Use();
    texture->Use();
    model->Draw();
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
    if (boxCollision != nullptr)
        delete boxCollision;

    delete transform;
}
