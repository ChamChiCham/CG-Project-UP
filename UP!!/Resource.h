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
protected:
	SBuffer		buffer;
	SShapeData*	data = nullptr;
	glm::vec3	color = glm::vec3(1.f);
	std::vector<glm::mat4>	mats;


protected:
	void setUniform(const SView& _view, const glm::mat4& _proj, const SLight& _light, const GLuint _program);
	void drawBuffer(const int _mode);

public:

	virtual void updateBuffer();

	void setData(const int _shape);

	virtual void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);

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
	void setMatrix(CShape& _other);
	const size_t getMatrixSize();
};

class CBrick : public CShape
{
private:
	glm::ivec3	pos = { 0, 0, 0 };
	int& x =	pos.x;
	int& y =	pos.y;
	int& z =	pos.z;

	static unsigned int texture;

private:
	void updateTexture();

public:

	CBrick();
	CBrick(const glm::ivec3& _pos);

	void updateBuffer() override;
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light) override;
	const glm::ivec3& getPos();
	void setPos(const int _y, const int _x, const int _z);
	void move(const int _dy, const int _dx, const int _dz);

};

class CPlayer
{
private:
	std::array<CShape, 3> shapes;
	
	int			status = PLAYER_STAND;

public:
	void init();
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);
	void changeStatus(const int _status);
	int& getstatus();
	CShape& getShape();
};

class CMap
{
private:
	std::vector<CBrick> bricks;


public:
	void init(const int _idx);
	void updateBuffer();
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);

	CBrick& operator()(const glm::ivec3 _pos);
	CBrick& operator()(const int _y, const int _x, const int _z);

	const bool isPosition(const int _y, const int _x, const int _z);

};