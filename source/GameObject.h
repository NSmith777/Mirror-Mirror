#pragma once

#include "Framework/Model.h"
#include "Framework/Texture.h"
#include "Framework/Transform.h"
#include "Framework/Shader.h"
#include "Framework/Camera.h"

class GameObject {
public:
	GameObject(Model *mdl, Texture* tex, Shader *shdr);
	~GameObject();

	void SetModel(Model* mdl) { model = mdl; }
	void SetTexture(Texture* tex) { texture = tex; }
	void SetShader(Shader* shdr) { shader = shdr; }

	Transform* GetTransform() { return &transform; }

	void Render(Camera *cam);

private:
	Shader* shader;
	Texture* texture;
	Model* model;

	Constants constants;
	Transform transform;
};
