#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <array>

#include "CShaderMgr.h"
#include "CShapeDataMgr.h"
#include "Define.h"
#include "Struct.h"

class CShape
{
private:
	SBuffer		buffer;
	SShapeData*	data = nullptr;
	glm::vec3	color = glm::vec3(1.f);

protected:
	std::vector<glm::mat4>	mats;


public:

	void updateBuffer();

	void setData(const int _shape);

	void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);

	void scale(const int _idx, const float _fir, const float _sec, const float _thi);
	void scale(const int _idx, const glm::vec3 _vec);

	void rotate(const int _idx, const float _deg, const float _fir, const float _sec, const float _thi);
	void rotate(const int _idx, const float _deg, const glm::vec3 _vec);
	void rotate(const int _idx, const float _x, const float _y, const float _z, const float _deg,
		const float _fir, const float _sec, const float _thi);
	void rotate(const int _idx, const glm::vec3 _axis, const float _deg, const glm::vec3 _vec);

	void translate(const int _idx, const float _fir, const float _sec, const float _thi);
	void translate(const int _idx, const glm::vec3 _vec);

	void setColor(const glm::vec3& _color);
	void setColor(const float _r, const float _g, const float _b);

	void clearMatrix(const int _idx);

};

class CBrick : public CShape
{
private:
	glm::ivec3	pos = { 0.f, 0.f, 0.f };
	int& x =	pos.x;
	int& y =	pos.y;
	int& z =	pos.z;

public:

	CBrick();
	CBrick(const glm::ivec3& _pos);

	void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);
	const glm::ivec3& getPos();

};

class CPlayer
{
private:
	std::array<CShape, 3> shapes;
	int status = PLAYER_STAND;
	glm::ivec3 pos = { 0.f, 0.f, 0.f };
	int& x = pos.x;
	int& y = pos.y;
	int& z = pos.z;

public:
	void init();
	void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);
};

class CMap
{
private:
	std::vector<CBrick> bricks;


public:
	void init(const int _idx);
	void updateBuffer();
	void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);

	CBrick& operator()(const glm::ivec3 _pos);
	CBrick& operator()(const int _y, const int _x, const int _z);

};