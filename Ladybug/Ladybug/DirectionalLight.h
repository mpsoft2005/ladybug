#pragma once

#include "Color.h"

class DirectionalLight
{
public:
	Color color = Color(1, 1, 1);
	float intensity = 1;

public:
	DirectionalLight();
	~DirectionalLight();
};
