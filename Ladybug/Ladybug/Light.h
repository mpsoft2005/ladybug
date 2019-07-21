#pragma once

#include <memory>

#include "Color.h"
#include "Transform.h"
#include "ShadowMap.h"

class World;

enum LightShadows
{
	SHADOW_NONE,
	SHADOW_HARD,
	SHADOW_SOFT,
};

class Light
{
public:
	std::unique_ptr<Transform> transform;
	Color color = Color(1, 1, 1);
	float intensity = 1;

	LightShadows shadows = LightShadows::SHADOW_NONE;
	float shadowBias = 0.05f;

private:
	std::shared_ptr<ShadowMap> shadowMap;

	void PrepareShadowMap(const World& world);

public:
	Light();
	~Light();

	Vector3 GetDirection();
	std::shared_ptr<ShadowMap> GetShadowMap(const World& world);
	float ShadowFactor(const World& world, const Vector3& v);
};
