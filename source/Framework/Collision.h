#pragma once

#include "Transform.h"

class BoxCollision {
public:
	BoxCollision(Transform* trans, XMFLOAT3 size);
	~BoxCollision();

	bool Ray_Intersect(XMFLOAT3 L1, XMFLOAT3 L2, XMFLOAT3& Hit);

private:
	Transform* transform;

	XMFLOAT3 box_size;
};
