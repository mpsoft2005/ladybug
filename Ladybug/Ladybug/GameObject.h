#pragma once

#include <vector>
#include <string>

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class GameObject
{
public:
	std::string name;
	Transform* transform = NULL;
	Mesh* mesh = NULL;
	Material* material = NULL;

private:
	std::vector<Component*> components;

public:
	GameObject();
	~GameObject();
};
