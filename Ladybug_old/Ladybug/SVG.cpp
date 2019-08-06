
#include <fstream>

#include "SVG.h"



SVG::SVG(int width, int height)
{
	this->width = width;
	this->height = height;
}


SVG::~SVG()
{
}

void SVG::DrawLine(float x1, float y1, float x2, float y2, const Color32& strokeColor, int strokeWidth)
{
	SvgLine line;
	line.x1 = x1;
	line.y1 = y1;
	line.x2 = x2;
	line.y2 = y2;
	line.strokeColor = strokeColor;
	line.strokeWidth = strokeWidth;
	lines.push_back(line);
}

void SVG::DrawRect(float x, float y, float width, float height, const Color32& strokeColor, int strokeWidth, const Color32& fillColor)
{
	SvgRect rect;
	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;
	rect.fillColor = fillColor;
	rect.strokeColor = strokeColor;
	rect.strokeWidth = strokeWidth;
	rects.push_back(rect);
}

void SVG::Save(const char* filename)
{

	std::ofstream ofs;
	ofs.open(filename);

	ofs << "<svg version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\">" << std::endl;

	for (int i = 0; i < (int)rects.size(); i++)
	{
		const SvgRect& rect = rects[i];
		ofs << "<rect x=\"" << rect.x << "\" y=\"" << rect.y << "\" width=\"" << rect.width << "\" height=\"" << rect.height
			<< "\" style=\"fill:rgb(" << (int)rect.fillColor.r << ", " << (int)rect.fillColor.g << ", " << (int)rect.fillColor.b << "); stroke-width:" << rect.strokeWidth
			<< "; stroke:rgb(" << (int)rect.strokeColor.r << ", " << (int)rect.strokeColor.g << ", " << (int)rect.strokeColor.b << ")\" />" << std::endl;
	}

	for (int i = 0; i < (int)lines.size(); i++)
	{
		const SvgLine& line = lines[i];
		ofs << "<line x1=\"" << line.x1 << "\" y1=\"" << line.y1 << "\" x2=\"" << line.x2 << "\" y2=\"" << line.y2
			<< "\" style=\"stroke:rgb(" << (int)line.strokeColor.r << ", " << (int)line.strokeColor.g << ", " << (int)line.strokeColor.b << ");stroke-width:" << line.strokeWidth << "\" />" << std::endl;
	}

	ofs << "</svg>\n";
	ofs.close();

}
