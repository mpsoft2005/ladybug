#pragma once

#include <string>

class Color;
class Color32;

class Bitmap
{
private:
	int width;
	int height;
	Color32* buffer;

public:
	Bitmap(int width, int height);
	~Bitmap();

	void SetPixel(int x, int y, const Color& color);
	void SetPixel(int x, int y, const Color32& color);
	void Save(const std::string& filename);
};

