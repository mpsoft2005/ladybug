#pragma once

#include <vector>
#include <memory>

#include "Pipeline.h"
#include "GameObject.h"

class Light;
class Camera;

class ShadowMap : PipelineListener
{
public:
	int width = 512;
	int height = 512;
	float shadowBias = 0.05f;
	std::unique_ptr<float[]> depthBuffer;

private:
	std::shared_ptr<Camera> camera;

private:
	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in);
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in);

public:
	ShadowMap(const Light& light);
	~ShadowMap();

	void Render(const std::vector< std::shared_ptr<GameObject> >& gameObjects);
	float ShadowFactor(const Vector3& v);
};

