#pragma once

#include <memory>
#include <string>
#include "Mesh.h"

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static std::string base_dir;
	static std::shared_ptr<Mesh> Load(const char* filename);
};
