#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class GameObject
{
public:
	std::string name;
	std::unique_ptr<Transform> transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

public:
	GameObject();
	~GameObject();
};
