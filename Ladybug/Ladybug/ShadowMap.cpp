#include "ShadowMap.h"

#include "Camera.h"
#include "Screen.h"
#include "Pipeline.h"
#include "Light.h"
#include "World.h"


ShadowMap::ShadowMap(std::shared_ptr<Light> light)
{
	this->light = light;
}

ShadowMap::~ShadowMap()
{

}

float ShadowMap::ShadowFactor(const Vector3& v)
{
	Vector3 vLightSpace = camera->WorldToScreenPoint(v);
	int shadowBufferIdx = (int)(std::roundf(vLightSpace.y) * width + std::roundf(vLightSpace.x));

	if (vLightSpace.z - shadowBias <= depthBuffer[shadowBufferIdx])
		return 1.f;
	else
		return 0.f;
}

void ShadowMap::Render(const std::vector< std::shared_ptr<GameObject> >& gameObjects)
{
	if (depthBuffer == nullptr)
	{
		depthBuffer = std::make_unique<float[]>(width * height);
	}

	camera = std::make_shared<Camera>();
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

	World world;
	world.camera = camera;
	world.gameObjects = gameObjects;

	Pipeline pipe;
	pipe.RegisterListener(this);
	pipe.Process(world, depthBuffer.get(), nullptr, width, height);
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