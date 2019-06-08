
#include <fstream>
#include <string>
#include <cassert>

#include "Bitmap.h"
#include "Color.h"
#include "Color32.h"


#pragma pack(push, 1)
struct BitmapFileHeader
{
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t off_bits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapInformationHeader
{
	uint32_t size;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bit_count;
	uint32_t compression;
	uint32_t size_image;
	uint32_t x_pels_per_meter;
	uint32_t y_pels_per_meter;
	uint32_t clr_used;
	uint32_t clr_important;
};
#pragma pack(pop)


Bitmap::Bitmap(int width, int height)
{
	assert(width > 0 && height > 0);
	this->width = width;
	this->height = height;
	buffer = new Color32[width * height];
}

Bitmap::~Bitmap()
{
	delete[] buffer;
}

void Bitmap::SetPixel(int x, int y, const Color& color)
{
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	Color32& c = buffer[y * width + x];
	c.r = (uint8_t)(color.r * 255);
	c.g = (uint8_t)(color.g * 255);
	c.b = (uint8_t)(color.b * 255);
	c.a = (uint8_t)(color.a * 255);
}

void Bitmap::SetPixel(int x, int y, const Color32& color)
{
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	buffer[y * width + x] = color;
}

void Bitmap::Save(const std::string& filename)
{
	const uint8_t BYTES_PER_PIXEL = 3;
	int row_increment_ = width * BYTES_PER_PIXEL;

	std::ofstream stream(filename, std::ios::binary);

	if (!stream)
	{
		printf("Cannot open file %s\n", filename.c_str());
		return;
	}

	BitmapInformationHeader bih;

	bih.width = width;
	bih.height = height;
	bih.bit_count = BYTES_PER_PIXEL << 3;
	bih.clr_important = 0;
	bih.clr_used = 0;
	bih.compression = 0;
	bih.planes = 1;
	bih.size = sizeof(bih);
	bih.x_pels_per_meter = 0;
	bih.y_pels_per_meter = 0;
	bih.size_image = (((bih.width * BYTES_PER_PIXEL) + 3) & 0x0000FFFC) * bih.height;

	BitmapFileHeader bfh;

	bfh.type = 19778;
	bfh.size = sizeof(bfh) + sizeof(bih) + bih.size_image;
	bfh.reserved1 = 0;
	bfh.reserved2 = 0;
	bfh.off_bits = sizeof(bih) + sizeof(bfh);

	stream.write((const char *)&bfh, sizeof(bfh));
	stream.write((const char *)&bih, sizeof(bih));

	unsigned int padding = (4 - ((3 * width) % 4)) % 4;
	char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Color32& c = buffer[y * width + x];
			stream << c.b << c.g << c.r;
		}
		stream.write(padding_data, padding);
	}

	stream.close();
}