#pragma once

#include <vector>
#include <memory>

#include "Pipeline.h"

class Camera;
class Light;
class GameObject;
class Color;

class World : PipelineListener
{
public:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Light> light;
	std::vector< std::shared_ptr<GameObject> > gameObjects;

	std::unique_ptr<Color[]> frameBuffer;
	std::unique_ptr<float[]> depthBuffer;

private:
	void PrepareBuffers();

	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in);
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in);

public:
	World();
	~World();

	void Render();
};

