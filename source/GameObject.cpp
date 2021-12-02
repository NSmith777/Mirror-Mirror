#include "GameObject.h"

GameObject::GameObject(Model* mdl, Texture* tex, Shader* shdr) {
    SetModel(mdl);
    SetTexture(tex);
    SetShader(shdr);

    boxCollision = NULL;
    transform = new Transform();

    ZeroMemory(&constants, sizeof(constants));
}

GameObject::GameObject(const GameObject& old) {
    model = old.model;
    texture = old.texture;
    shader = old.shader;

    boxCollision = old.boxCollision;
    transform = old.transform;

    memcpy(&constants, &old.constants, sizeof(constants));
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

}
