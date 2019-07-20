#pragma once

#include "Pipeline.h"

class Camera;
class DirectionalLight;

class ShadowMap : PipelineListener
{
public:
	int width = 512;
	int height = 512;
	float *depthBuffer = 0;

private:
	DirectionalLight* light;
	Camera* camera = nullptr;

private:
	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in);
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in);

public:
	ShadowMap(DirectionalLight* light);
	~ShadowMap();

	void Render(const World& world);
};

