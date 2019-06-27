#pragma once

class GameObject;

class Component
{
public:
	GameObject* gameObject = 0;

public:
	Component();
	virtual ~Component();
};

