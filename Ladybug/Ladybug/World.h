#pragma once

#include <vector>
#include <memory>

class Camera;
class Light;
class GameObject;
class Color;

class World
{
public:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Light> light;
	std::vector< std::shared_ptr<GameObject> > gameObjects;

	Color *frameBuffer = 0;
	float *depthBuffer = 0;

public:
	World();
	~World();

	void Render();
};

