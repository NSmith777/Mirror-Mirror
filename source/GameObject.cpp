#include "GameObject.h"

GameObject::GameObject(Model* mdl, Texture* tex, Shader* shdr) {
    SetModel(mdl);
    SetTexture(tex);
    SetShader(shdr);
    ZeroMemory(&constants, sizeof(constants));
}

void GameObject::Render(Camera* cam) {
    constants.MVP = transform.GetModelMatrix() * cam->GetViewMatrix() * cam->GetProjMatrix();

    shader->SetConstants(&constants);

    shader->Use();
    texture->Use();
    model->Draw();
}

GameObject::~GameObject() {

}
