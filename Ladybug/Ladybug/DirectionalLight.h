#pragma once

#include "Color.h"
#include "Transform.h"

class DirectionalLight
{
public:
	Transform* transform = 0;
	Color color = Color(1, 1, 1);
	float intensity = 1;

public:
	DirectionalLight();
	~DirectionalLight();

	Vector3 getDirection();
};
