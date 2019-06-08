#include "Color.h"

Color::Color()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 1;
}

Color::Color(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1;
}

Color::~Color()
{
}
