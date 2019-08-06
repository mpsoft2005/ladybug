#pragma once

#include <iostream>

class Color32
{
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

public:
	Color32();
	Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	Color32(uint8_t r, uint8_t g, uint8_t b);
	~Color32();
};

