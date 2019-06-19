#pragma once

#include <string>

#include "Mesh.h"
#include "Material.h"

class GameObject
{
public:
	std::string name;
	Mesh* mesh = NULL;
	Material* material = NULL;

public:
	GameObject();
	~GameObject();
};
