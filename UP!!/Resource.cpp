#include "Resource.h"

// --
// CShape member function
// --

void CShape::updateBuffer()
{
	if (data == nullptr) {
		std::cerr << "data isn't set" << std::endl;
		return;
	}

	if (buffer.VAO != 0) {
		glDeleteVertexArrays(1, &buffer.VAO);
		glDeleteBuffers(2, buffer.VBO);
		glDeleteBuffers(1, &buffer.EBO);
	}



	glGenBuffers(2, buffer.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, data->coords.size() * sizeof(GLfloat), data->coords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, data->normals.size() * sizeof(GLfloat), data->normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &buffer.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(unsigned int), data->indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &buffer.VAO);
	glBindVertexArray(buffer.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
}

void CShape::setData(const int _shape)
{
	data = &CShapeDataMgr::getInst()->getData(_shape);
}

void CShape::draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	glUseProgram(_program);

	// light
	unsigned int lightPosLocation = glGetUniformLocation(_program, "lightPos");
	glUniform3f(lightPosLocation, _light.x, _light.y, _light.z);

	unsigned int lightColorLocation = glGetUniformLocation(_program, "lightColor");
	glUniform3f(lightColorLocation, _light.r, _light.g, _light.b);

	unsigned int objColorLocation = glGetUniformLocation(_program, "objectColor");
	glUniform3f(objColorLocation, color.r, color.g, color.b);

	unsigned int viewPosLocation = glGetUniformLocation(_program, "viewPos");
	glUniform3f(viewPosLocation, _view.eye.x, _view.eye.y, _view.eye.z);

	glBindVertexArray(buffer.VAO);

	glm::mat4 MM = glm::mat4(1.f);
	for (auto& mat : mats) {
		MM = mat * MM;
	}

	glm::mat4 MV = glm::lookAt(_view.eye, _view.at, _view.up);
	glm::mat4 MP = _proj;

	// uniform matrix 
	unsigned int transformLocation = glGetUniformLocation(_program, "model");
	unsigned int viewLocation = glGetUniformLocation(_program, "view");
	unsigned int projectionLocation = glGetUniformLocation(_program, "projection");

	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &MM[0][0]);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &MP[0][0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
	glDrawElements(_mode, data->indices.size(), GL_UNSIGNED_INT, 0);
}

void CShape::scale(const int _idx, const float _fir, const float _sec, const float _thi)
{
	scale(_idx, glm::vec3(_fir, _sec, _thi));
}

void CShape::scale(const int _idx, const glm::vec3 _vec)
{
	while (mats.size() <= _idx) {
		mats.push_back(glm::mat4(1.f));
	}

	mats[_idx] = glm::scale(mats[_idx], _vec);
}

void CShape::rotate(const int _idx, const float _deg, const float _fir, const float _sec, const float _thi)
{
	rotate(_idx, _deg, glm::vec3(_fir, _sec, _thi));
}

void CShape::rotate(const int _idx, const float _deg, const glm::vec3 _vec)
{
	while (mats.size() <= _idx) {
		mats.push_back(glm::mat4(1.f));
	}

	mats[_idx] = glm::rotate(mats[_idx], glm::radians(_deg), _vec);
}

void CShape::rotate(const int _idx, const float _x, const float _y, const float _z, const float _deg, const float _fir, const float _sec, const float _thi)
{
	rotate(_idx, glm::vec3(_x, _y, _z), _deg, glm::vec3(_fir, _sec, _thi));
}

void CShape::rotate(const int _idx, const glm::vec3 _axis, const float _deg, const glm::vec3 _vec)
{
	while (mats.size() <= _idx) {
		mats.push_back(glm::mat4(1.f));
	}

	glm::mat4 m = glm::mat4(1.f);
	m = glm::translate(glm::mat4(1.f), -_axis) * m;
	m = glm::rotate(glm::mat4(1.f), glm::radians(_deg), _vec) * m;
	m = glm::translate(glm::mat4(1.f), _axis) * m;
	mats[_idx] = m * mats[_idx];
}

void CShape::translate(const int _idx, const float _fir, const float _sec, const float _thi)
{
	translate(_idx, glm::vec3(_fir, _sec, _thi));
}

void CShape::translate(const int _idx, const glm::vec3 _vec)
{
	while (mats.size() <= _idx) {
		mats.push_back(glm::mat4(1.f));
	}

	mats[_idx] = glm::translate(mats[_idx], _vec);
}

void CShape::setColor(const glm::vec3& _color)
{
	color = _color;
}

void CShape::setColor(const float _r, const float _g, const float _b)
{
	setColor(glm::vec3(_r, _g, _b));
}

void CShape::clearMatrix(const int _idx)
{
	if (mats.size() <= _idx)
		return;
	mats[_idx] = glm::mat4(1.f);
}

// --
// CBrick member function
// --

CBrick::CBrick()
{
	setData(SHAPE_DICE);
	setColor(0.f, 1.f, 0.f);
}

void CBrick::draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	translate(static_cast<int>(mats.size()), pos);
	CShape::draw(_program, _view, _proj, _mode, _light);
	mats.pop_back();
}
