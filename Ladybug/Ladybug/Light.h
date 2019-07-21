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

	std::unique_ptr<ShadowMap> shadowMap;
	LightShadows shadows = LightShadows::SHADOW_NONE;
	float shadowBias = 0.05f;


public:
	Light();
	~Light();

	Vector3 getDirection();
	float ShadowFactor(const World& world, const Vector3& v);
};
