#pragma once

#include <vector>
#include "Color32.h"

struct SvgRect
{
	float x;
	float y;
	float width;
	float height;
	Color32 fillColor;
	Color32 strokeColor;
	int strokeWidth;
};

struct SvgLine
{
	float x1, y1;
	float x2, y2;
	Color32 strokeColor;
	int strokeWidth;
};

class SVG
{
private:
	int width;
	int height;
	std::vector<SvgRect> rects;
	std::vector<SvgLine> lines;

public:
	SVG(int width, int height);
	~SVG();

	void DrawLine(float x1, float y1, float x2, float y2, const Color32& strokeColor, int strokeWidth);
	void DrawRect(float x, float y, float width, float height, const Color32& strokeColor, int strokeWidth, const Color32& fillColor);

	void Save(const char* filename);
};
