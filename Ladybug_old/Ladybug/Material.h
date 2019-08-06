#pragma once

#include "Color.h"

class Material
{
public:
	Color albedo;
	Color specular;
	float specularGloss;

public:
	Material();
	~Material();
};

