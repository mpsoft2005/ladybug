#include "ShadowMap.h"

#include "Camera.h"
#include "Screen.h"
#include "Pipeline.h"
#include "DirectionalLight.h"


ShadowMap::ShadowMap(DirectionalLight* light)
{
	this->light = light;
}


ShadowMap::~ShadowMap()
{
	delete camera;
	delete depthBuffer;
}

void ShadowMap::Render(const World& world)
{
	if (depthBuffer == nullptr)
	{
		depthBuffer = new float[width * height];
	}

	camera = new Camera();
	camera->transform->localPosition = light->transform->localPosition;
	camera->transform->localEulerAngles = light->transform->localEulerAngles;

	// TODO: remove hard-coded params of camera
	camera->orthographic = true;
	camera->orthographicSize = 7;
	camera->nearClipPlane = 0.3f;
	camera->farClipPlane = 20;

	for (int i = 0; i < width * height; i++)
	{
		// clear z buffer
		depthBuffer[i] = camera->farClipPlane;
	}

	Pipeline pipe;
	pipe.RegisterListener(this);
	pipe.Process(world, depthBuffer, nullptr, width, height);
}

v2f ShadowMap::OnProcessVertex(const Pipeline& pipe, const a2v& in)
{
	v2f out;

	out.pos = camera->WorldToScreenPoint(in.vertex);
	out.worldPos = in.vertex;

	return out;
}

Color ShadowMap::OnProcessFragment(const Pipeline& pipe, const v2f& in)
{
	Color out;

	return out;
}