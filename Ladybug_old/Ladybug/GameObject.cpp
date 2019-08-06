#include "GameObject.h"



GameObject::GameObject()
{
	transform = std::make_unique<Transform>();
}


GameObject::~GameObject()
{

}
