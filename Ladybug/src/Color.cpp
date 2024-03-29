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

Color Color::operator+(const Color& b) const
{
	return Color(this->r + b.r, this->g + b.g, this->b + b.b, this->a + b.a);
}

Color Color::operator*(float b) const
{
	return Color(this->r * b, this->g * b, this->b * b, this->a * b);
}

Color Color::operator*(const Color& b) const
{
	return Color(this->r * b.r, this->g * b.g, this->b * b.b, this->a * b.a);
}

Color Color::operator/(float b) const
{
	return Color(this->r / b, this->g / b, this->b / b, this->a / b);
}
