#pragma once

#include <memory>

#include "Light.h"
#include "Pipeline.h"

class Camera;

class ShadowMap : PipelineListener
{
public:
	int width = 512;
	int height = 512;
	std::unique_ptr<float[]> depthBuffer;

private:
	std::shared_ptr<Light> light;
	std::shared_ptr<Camera> camera;

private:
	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in);
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in);

public:
	ShadowMap(std::shared_ptr<Light> light);
	~ShadowMap();

	void Render(const World& world);
};

