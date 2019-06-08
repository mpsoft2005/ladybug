#pragma once
class Color
{
public:
	float r;
	float g;
	float b;
	float a; // Alpha component of the color (0 is transparent, 1 is opaque).

public:
	Color();
	Color(float r, float g, float b, float a);
	Color(float r, float g, float b);
	~Color();
};

