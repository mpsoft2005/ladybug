#pragma once

#include "Pipeline.h"

class Camera;
class Light;

class ShadowMap : PipelineListener
{
public:
	int width = 512;
	int height = 512;
	float *depthBuffer = 0;

private:
	Light* light;
	Camera* camera = nullptr;

private:
	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in);
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in);

public:
	ShadowMap(Light* light);
	~ShadowMap();

	void Render(const World& world);
};

