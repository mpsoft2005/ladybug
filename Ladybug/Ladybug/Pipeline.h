#pragma once

class Color;
class Vector3;
class World;
class Pipeline;

struct a2v
{

};

struct v2f
{

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
	World* world = 0;
	PipelineListener* listener = 0;

public:
	Pipeline();
	~Pipeline();

	void RegisterListener(PipelineListener* listener);
	void Process();
};

