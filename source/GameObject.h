//==============================================================================
// File: GameObject.h
// 
// Description: Declares the GameObject class.
// 
//==============================================================================

#pragma once

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>

#include "Framework/GfxDevice.h"
#include "Framework/Texture.h"
#include "Framework/Model.h"
#include "Framework/Collision.h"

//=============================================================================
// 
// Synopsis:    
// 
//=============================================================================
class GameObject {
public:
	GameObject(Model *mdl, Texture* tex, Shader *shdr);
	~GameObject();

	void SetModel(Model* mdl) { model = mdl; }
	void SetTexture(Texture* tex) { texture = tex; }
	void SetShader(Shader* shdr) { shader = shdr; }

	void AddBoxCollider(XMFLOAT3 size);

	BoxCollision* GetBoxCollision() { return boxCollision; }
	Transform* GetTransform() { return transform; }

	void Render(Camera *cam);

	// Supress heap alignment warnings
	void* operator new(size_t i) { return _mm_malloc(i, 16); }
	void operator delete(void* p) { _mm_free(p); }

private:
	Shader* shader;
	Texture* texture;
	Model* model;
	BoxCollision* boxCollision;
	Transform *transform;

	Constants constants;
};
