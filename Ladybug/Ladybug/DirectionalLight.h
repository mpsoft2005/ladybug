#pragma once

#include "Color.h"

class DirectionalLight
{
public:
	Color color;
	float intensity;

public:
	DirectionalLight();
	~DirectionalLight();
};
