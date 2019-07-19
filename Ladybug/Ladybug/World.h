#pragma once

#include <vector>

class Camera;
class DirectionalLight;
class GameObject;
class Color;

class World
{
public:
	Camera* camera = 0;
	DirectionalLight* directionalLight = 0;
	std::vector<GameObject*> gameObjects;

	Color *frameBuffer = 0;
	float *depthBuffer = 0;

public:
	World();
	~World();

	void Render();
};
