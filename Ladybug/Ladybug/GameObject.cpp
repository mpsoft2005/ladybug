#include "GameObject.h"



GameObject::GameObject()
{
	transform = new Transform();
}


GameObject::~GameObject()
{
	delete transform;
	delete mesh;
	delete material;
}
