#include "CShapeDataMgr.h"

#include <fstream>
#include <iostream>
#include <string>
#include <list>

#include "Define.h"

CShapeDataMgr* CShapeDataMgr::instance = nullptr;

SShapeData::SShapeData()
{}


CShapeDataMgr::CShapeDataMgr()
{
	init();
}

void CShapeDataMgr::init()
{
	data.resize(SHAPE_MAX);
	data[SHAPE_DICE].coords =
	{
		-0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f,		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,		-0.5f,  0.5f, -0.5f,	-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,	0.5f, -0.5f,  0.5f,		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,	-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,	-0.5f,  0.5f, -0.5f,	-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,	-0.5f, -0.5f,  0.5f,	-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		0.5f,  0.5f, -0.5f,		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,		0.5f, -0.5f,  0.5f,		0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f,		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,		-0.5f, -0.5f,  0.5f,	-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,	0.5f,  0.5f, -0.5f,		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,		-0.5f,  0.5f,  0.5f,	-0.5f,  0.5f, -0.5f
	};
	data[SHAPE_DICE].normals =
	{
		0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,
		  -1.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0
	};

	data[SHAPE_DICE].textures = {
		0.f, 1.f / 2.f, 1.f / 3.f, 1.f / 2.f, 1.f / 3.f, 0.f,
		1.f / 3.f, 0.f, 0.f, 0.f, 0.f, 1.f / 2.f,

		1.f / 3.f, 0.f, 2.f / 3.f, 0.f, 2.f / 3.f, 1.f / 2.f,
		2.f / 3.f, 1.f / 2.f, 1.f / 3.f, 1.f / 2.f, 1.f / 3.f, 0.f,

		1.f, 0.f, 2.f / 3.f, 0.f, 2.f / 3.f, 1.f / 2.f,
		2.f / 3.f, 1.f / 2.f, 1.f, 1.f / 2.f, 1.f, 0.f,

		0.f, 1.f / 2.f, 1.f / 3.f, 1.f / 2.f, 1.f / 3.f, 1.f,
		1.f / 3.f, 1.f, 0.f, 1.f, 0.f, 1.f / 2.f,

		1.f / 3.f, 1.f / 2.f, 2.f / 3.f, 1.f / 2.f, 2.f / 3.f, 1.f,
		2.f / 3.f, 1.f, 1.f / 3.f, 1.f, 1.f / 3.f, 1.f / 2.f,

		2.f / 3.f, 1.f / 2.f, 1.f, 1.f / 2.f, 1.f, 1.f,
		1.f, 1.f, 2.f / 3.f, 1.f, 2.f / 3.f, 1.f / 2.f

	};


	data[SHAPE_SQUARE_PRAMID].coords =
	{
		0.f,	1.f,	0.f,
		1.f,	-1.f,	1.f,
		-1.f,	-1.f,	1.f,
		1.f,	-1.f,	-1.f,
		-1.f,	-1.f,	-1.f,
	};

	data[SHAPE_SQUARE_PRAMID].normals =
	{
		0.f, 1.f, 0.f,
		0.5773502692f, 0.5773502692f, -0.5773502692f,
		-0.5773502692f, 0.5773502692f, -0.5773502692f,
		0.5773502692f, -0.5773502692f, -0.5773502692f,
		-0.5773502692f, -0.5773502692f, -0.5773502692f,
	};

	data[SHAPE_SQUARE_PRAMID].indices =
	{
		0, 2, 1,
		0, 4, 2,
		0, 3, 4,
		0, 1, 3,
		1, 2, 4,
		1, 4, 3
	};


	data[SHAPE_SQUARE].coords =
	{
		1.f,	0.f,	1.f,
		-1.f,	0.f,	1.f,
		1.f,	0.f,	-1.f,
		-1.f,	0.f,	-1.f
	};

	data[SHAPE_SQUARE].normals =
	{
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};

	data[SHAPE_SQUARE].indices =
	{
		0, 2, 1,
		1, 2, 3
	};

	data[SHAPE_TRIANGLE].coords =
	{
		1.f,	0.f,	1.f,
		-1.f,	0.f,	1.f,
		0.f,	0.f,	-1.f,
	};

	data[SHAPE_TRIANGLE].normals =
	{
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
	};

	data[SHAPE_TRIANGLE].indices =
	{
		0, 2, 1
	};

	initObj("obj\\sphere.obj", SHAPE_SPHERE);
	initObj("obj\\player_stand.obj", SHAPE_PLAYER_STAND);
	initObj("obj\\player_hold.obj", SHAPE_PLAYER_HOLD);
	initObj("obj\\player_hang.obj", SHAPE_PLAYER_HANG);

}	

void CShapeDataMgr::initObj(const char* _obj, const int _shape)
{
	std::ifstream file(_obj);
	std::string line;

	if (!file.is_open()) {
		std::cerr << "ERROR: file open failed named \n" << _obj << std::endl;
		return;
	}

	std::vector<float> normals_data;
	std::list<int> normals_indices;

	data.push_back(SShapeData());
	while (std::getline(file, line)) {
		if (line.substr(0, 2) == "v ") {
			float vertices[3] = {};
			sscanf_s(line.c_str(), "v %f %f %f", &vertices[0], &vertices[1], &vertices[2]);

			for (auto& vertex : vertices)
				data[_shape].coords.push_back(vertex);
		}

		else if (line.substr(0, 3) == "vn ") {
			float normals[3] = {};
			sscanf_s(line.c_str(), "vn %f %f %f", &normals[0], &normals[1], &normals[2]);
			for (auto& normal : normals)
				normals_data.push_back(normal);
		}

		else if (line.substr(0, 2) == "f ") {
			if (data[_shape].normals.empty())
				data[_shape].normals.resize(data[_shape].coords.size());
			int indices[4] = { -1, -1, -1, -1 }, dummy0[4] = {}, normals[4] = {-1, -1, -1, -1};
			sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&indices[0], &dummy0[0], &normals[0],
				&indices[1], &dummy0[1], &normals[1],
				&indices[2], &dummy0[2], &normals[2],
				&indices[3], &dummy0[3], &normals[3]
			);

			for (int i = 0; i < 4; ++i) {
				if (i == 3 && indices[3] == -1)
					break;
				data[_shape].normals[(indices[i] - 1) * 3] += normals_data[(normals[i] - 1) * 3];
				data[_shape].normals[(indices[i] - 1) * 3 + 1] += normals_data[(normals[i] - 1) * 3 + 1];
				data[_shape].normals[(indices[i] - 1) * 3 + 2] += normals_data[(normals[i] - 1) * 3 + 2];
			}


			for (int i = 0; i < 3; ++i) {
				data[_shape].indices.push_back(indices[i] - 1);
			}
			if (indices[3] != -1) {
				data[_shape].indices.push_back(indices[0] - 1);
				data[_shape].indices.push_back(indices[2] - 1);
				data[_shape].indices.push_back(indices[3] - 1);
			}
		}
	}

	file.close();
}

SShapeData& CShapeDataMgr::getData(const int _idx)
{
	return data[_idx];
}

CShapeDataMgr* CShapeDataMgr::getInst()
{
	if (!instance) {
		instance = new CShapeDataMgr();
	}
	return instance;
}
