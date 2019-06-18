
#include <vector>
#include <map>
#include <assert.h>

#include "ObjLoader.h"

struct Vertex
{
	int v;
	int vt;
	int vn;

	bool operator < (const Vertex& rhs)  const
	{
		if (v < rhs.v)
			return true;
		if (v > rhs.v)
			return false;

		if (vt < rhs.vt)
			return true;
		if (vt > rhs.vt)
			return false;

		if (vn < rhs.vn)
			return true;
		if (vn > rhs.vn)
			return false;

		return false;
	}
};

ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

inline void ParseVertex(const char* buf, Vertex& vert)
{
	int data[3] = {0, 0, 0};
	int dataIdx = 0;

	int len = (int)strlen(buf);
	for (int i = 0; i < len; i++)
	{
		if (i == 0)
		{
			sscanf(&buf[i], "%d", &data[dataIdx]);
		}
		else if (buf[i] == '/')
		{
			dataIdx++;
			sscanf(&buf[i + 1], "%d", &data[dataIdx]);
		}
	}

	vert.v = data[0];
	vert.vt = data[1];
	vert.vn = data[2];
}

void DebugVert(const Vertex& vert)
{
	printf(" %d", vert.v);
	if (vert.vt > 0)
	{
		printf("/%d", vert.vt);
		if (vert.vn > 0)
		{
			printf("/%d", vert.vn);
		}
	}
	else if (vert.vn > 0)
	{
		printf("//%d", vert.vn);
	}
}

void DebugTri(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
	printf("f");
	DebugVert(v0);
	DebugVert(v1);
	DebugVert(v2);
	printf("\n");
}

Mesh* ObjLoader::Load(const char* filename)
{
	FILE * file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Cannot open obj file:%s\n", filename);
		return NULL;
	}

	Mesh* mesh = new Mesh();
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uv;
	std::vector<Color> colors;
	std::vector<Vertex> triangles;

	while (1)
	{
		static char line[1024];
		int res = fscanf(file, "%s", line);
		if (res == EOF)
			break;

		if (strcmp(line, "v") == 0)
		{
			Vector3 v;
			fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);
			vertices.push_back(v);
		}
		else if (strcmp(line, "vt") == 0)
		{
			Vector2 vt;
			fscanf(file, "%f %f", &vt.x, &vt.y);
			uv.push_back(vt);
		}
		else if (strcmp(line, "vn") == 0)
		{
			Vector3 vn;
			fscanf(file, "%f %f %f", &vn.x, &vn.y, &vn.z);
			normals.push_back(vn);
		}
		else if (strcmp(line, "f") == 0)
		{
			Vertex v0, v1, v2;
			fscanf(file, "%s", line);
			ParseVertex(line, v0);

			fscanf(file, "%s", line);
			ParseVertex(line, v1);

			fscanf(file, "%s", line);
			ParseVertex(line, v2);

			triangles.push_back(v0);
			triangles.push_back(v1);
			triangles.push_back(v2);
		}
		else
		{
			fgets(line, sizeof(line), file);
		}
	}

	std::map<Vertex, int> vert2idx;
	std::map<Vertex, int>::iterator it;

	for (int i = 0; i < (int)triangles.size(); i++)
	{
		const Vertex& vert = triangles[i];
		it = vert2idx.find(vert);
		int idx;

		if (it == vert2idx.end())
		{
			assert(vert.v > 0);
			idx = (int)mesh->vertices.size();
			mesh->vertices.push_back(vertices[vert.v - 1]);

			if (vert.vt > 0)
			{
				mesh->uv.push_back(uv[vert.vt - 1]);
				assert(mesh->uv.size() == mesh->vertices.size());
			}

			if (vert.vn > 0)
			{
				mesh->normals.push_back(normals[vert.vn - 1]);
				assert(mesh->normals.size() == mesh->vertices.size());
			}

			vert2idx[vert] = idx;
		}
		else
		{
			idx = it->second;
		}
		mesh->triangles.push_back(idx);
	}

	return mesh;
}
