#pragma once

#include "Vector2.h"
#include "Vector3.h"

class Color;
class World;
class Pipeline;

struct a2v
{
	Vector3 vertex;
	Vector3 normal;
	Vector2 uv;
};

struct v2f
{
	Vector3 pos;
	Vector3 worldPos;
	Vector3 normal;
	Vector2 uv;
};

class PipelineListener
{
public:
	virtual v2f OnProcessVertex(const Pipeline& pipe, const a2v& in) = 0;
	virtual Color OnProcessFragment(const Pipeline& pipe, const v2f& in) = 0;
};

class Pipeline
{
private:
	PipelineListener* listener = 0;

public:
	Pipeline();
	~Pipeline();

	void RegisterListener(PipelineListener* listener);
	void Process(const World& world, float *depthBuffer, Color *frameBuffer, int screenWidth, int screenHeight);
};

