#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
	delete mesh;
	delete material;
}
