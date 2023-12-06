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

unsigned int createTexture(const char* _name);

class CShape
{
private:
	SBuffer		buffer;
	SShapeData*	data = nullptr;
	glm::vec3	color = glm::vec3(1.f);
	std::vector<glm::mat4>	mats;

public:
	virtual ~CShape();


protected:
	void setUniform(const SView& _view, const glm::mat4& _proj, const SLight& _light, const GLuint _program);
	void drawBuffer(const int _mode);
	void updateTextureBuffer();

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
	const size_t getMatrixSize() const;
	void popMatrix();
};

class CBackground : public CShape
{
private:
	unsigned int texture;

public:
	~CBackground();

	void updateBuffer() override;
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light) override;
};

class CBrick : public CShape
{
private:
	glm::ivec3	pos = { 0, 0, 0 };
	int&		x =	pos.x;
	int&		y =	pos.y;
	int&		z =	pos.z;

	int			type = BRICK_TYPE_NORMAL;

	static std::array<unsigned int, BRICK_TYPE_MAX> textures;

private:
	void updateTextures();

public:

	CBrick();
	CBrick(const glm::ivec3& _pos);

	void updateBuffer() override;
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light) override;
	const glm::ivec3& getPos() const;
	void setPos(const int _y, const int _x, const int _z);
	void move(const int _dy, const int _dx, const int _dz);

	void setType(const int _type);
	const int getType() const;
};

class CPlayer
{
private:
	std::array<CShape, 4> shapes;
	
	int status = PLAYER_STAND;

public:
	void updateBuffer();
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
	const bool createBrick(const int _y, const int _x, const int _z, const int _type);

	CBrick& operator()(const glm::ivec3 _pos);
	CBrick& operator()(const int _y, const int _x, const int _z);

	const bool isPosition(const int _y, const int _x, const int _z) const;

};

class CLava
{
private:

	std::array<CShape, 49> shapes;
	glm::vec3 pos = {0.f, -10.f, 0.f};
	int speed = 0;

public:
	void updateBuffer();
	void reset();
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light);
	void move(const float _dx, const float _dz);
	const int getY() const;
	void setSpeed(const int _speed);
	void update();

};

class CItem : public CShape
{
private:
	glm::ivec3 pos = { 0.f, 0.f, 0.f };
	bool size_dir = false;
	int size_time = 50;

public:
	void updateBuffer() override;
	void draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light) override;
	void update();
	void setPos(const int _y, const int _x, const int _z);

};
