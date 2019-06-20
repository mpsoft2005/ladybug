#pragma once

#include <vector>

#include "Vector2.h"
#include "Vector3.h"
#include "Color.h"

class Mesh
{
public:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uv;
	std::vector<Color> colors;
	std::vector<int> triangles;

public:
	Mesh();
	~Mesh();

	void Debug();
	void DebugTri();
};
