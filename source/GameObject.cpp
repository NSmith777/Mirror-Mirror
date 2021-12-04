#include "GameObject.h"

GameObject::GameObject(Model* mdl, Texture* tex, Shader* shdr) {
    SetModel(mdl);
    SetTexture(tex);
    SetShader(shdr);

    boxCollision = nullptr;
    transform = new Transform();

    ZeroMemory(&constants, sizeof(constants));
}

void GameObject::AddBoxCollider(XMFLOAT3 size) {
    boxCollision = new BoxCollision(transform, size);
}

void GameObject::Render(Camera* cam) {
    constants.MVP = transform->GetModelMatrix() * cam->GetViewMatrix() * cam->GetProjMatrix();

    shader->SetConstants(&constants);

    shader->Use();
    texture->Use();
    model->Draw();
}

GameObject::~GameObject() {
    if (boxCollision != nullptr)
        delete boxCollision;

    delete transform;
}
