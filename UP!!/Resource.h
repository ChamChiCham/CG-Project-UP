#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "CShaderMgr.h"
#include "CShapeDataMgr.h"
#include "Define.h"
#include "Struct.h"

class CShape
{
private:
	SBuffer					buffer;
	SShapeData* data = nullptr;
	glm::vec3				color = glm::vec3(1.f);

protected:
	std::vector<glm::mat4>	mats;


public:

	void updateBuffer();

	void setData(const int _shape);

	virtual void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);

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
public:

	glm::ivec3 pos = { 0.f, 0.f, 0.f };
	int& x = pos.x;
	int& y = pos.y;
	int& z = pos.z;
	
	CBrick();
	CBrick(const glm::ivec3& _pos);

	void draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light) override;

};

class CPlayer
{
	
};

class CMap
{
	// 임시로 bricks를 public 한정자로, vector<CBrick> 자료형으로 만들었다.
	// 자료구조와 한정자는 추후 수정할 것이다.
public:
	std::vector<CBrick> bricks;


	void createBrick();
	void createBricks(const std::vector<int>& _data);

	CBrick& operator()(const glm::ivec3 _pos);
	CBrick& operator()(const int _y, const int _x, const int _z);

};
