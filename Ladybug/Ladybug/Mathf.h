#pragma once

#include <math.h>
#include <algorithm>

class Mathf
{
public:
	static const float PI;
	static const float Deg2Rad;
	static const float Rad2Deg;

public:
	Mathf();
	~Mathf();

	static inline float Min(float a, float b, float c)
	{
		return std::min(a, std::min(b, c));
	}

	static inline float Max(float a, float b, float c)
	{
		return std::max(a, std::max(b, c));
	}
};

