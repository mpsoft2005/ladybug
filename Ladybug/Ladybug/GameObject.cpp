#include "GameObject.h"



GameObject::GameObject()
{
	transform = new Transform();
	transform->gameObject = this;
}


GameObject::~GameObject()
{
	for (int i = 0; i < (int)components.size(); i++)
	{
		delete components[i];
	}
	delete transform;
	delete mesh;
	delete material;
}
