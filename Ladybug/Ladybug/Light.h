#pragma once

#include <memory>

#include "Color.h"
#include "Transform.h"

class Light
{
public:
	std::unique_ptr<Transform> transform;
	Color color = Color(1, 1, 1);
	float intensity = 1;

public:
	Light();
	~Light();

	Vector3 getDirection();
};
