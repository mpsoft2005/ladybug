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

	template <class T>
	T* AddComponent()
	{
		T* t = new T;
		Component* c = dynamic_cast<Component*>(t);

		if (c != NULL)
		{
			c->gameObject = this;
			components.push_back(c);
			return t;
		}
		else
		{
			delete t;
			return NULL;
		}
	}

	template <class T>
	T* GetComponent()
	{
		for (int i = 0; i < (int)components.size(); i++)
		{
			T* t = dynamic_cast<T*>(components[i]);
			if (t != NULL)
			{
				return t;
			}
		}
		return NULL;
	}
};
