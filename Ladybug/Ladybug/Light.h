#pragma once

#include "Color.h"
#include "Transform.h"

class Light
{
public:
	Transform* transform = 0;
	Color color = Color(1, 1, 1);
	float intensity = 1;

public:
	Light();
	~Light();

	Vector3 getDirection();
};
