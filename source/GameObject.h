#pragma once

#include "Framework/Model.h"
#include "Framework/Texture.h"
#include "Framework/Transform.h"
#include "Framework/Shader.h"
#include "Framework/Collision.h"
#include "Framework/Camera.h"

class GameObject {
public:
	GameObject(Model *mdl, Texture* tex, Shader *shdr);
	GameObject(const GameObject& old); // Copy constructor
	~GameObject();

	void SetModel(Model* mdl) { model = mdl; }
	void SetTexture(Texture* tex) { texture = tex; }
	void SetShader(Shader* shdr) { shader = shdr; }

	void AddBoxCollider(XMFLOAT3 size);

	BoxCollision* GetBoxCollision() { return boxCollision; }
	Transform* GetTransform() { return transform; }

	void Render(Camera *cam);

private:
	Shader* shader;
	Texture* texture;
	Model* model;
	BoxCollision* boxCollision;
	Transform *transform;

	Constants constants;
};
