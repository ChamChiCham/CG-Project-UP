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
		if (!data->indices.empty())
			glDeleteBuffers(1, &buffer.EBO);
	}



	glGenBuffers(2, buffer.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, data->coords.size() * sizeof(GLfloat), data->coords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, data->normals.size() * sizeof(GLfloat), data->normals.data(), GL_STATIC_DRAW);

	if (!data->indices.empty()) {
		glGenBuffers(1, &buffer.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->indices.size() * sizeof(unsigned int), data->indices.data(), GL_STATIC_DRAW);
	}
	glGenVertexArrays(1, &buffer.VAO);
	glBindVertexArray(buffer.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	if (!data->indices.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
	}
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

	if (data->indices.empty()) {
		glDrawArrays(_mode, 0, static_cast<GLsizei>(data->coords.size() / 3));
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
		glDrawElements(_mode, static_cast<GLsizei>(data->indices.size()), GL_UNSIGNED_INT, 0);
	}
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

	glm::mat4 m = glm::scale(glm::mat4(1.f), _vec);
	mats[_idx] = m * mats[_idx];
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

	glm::mat4 m = glm::rotate(glm::mat4(1.f), glm::radians(_deg), _vec);
	mats[_idx] = m * mats[_idx];
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
	glm::mat4 m = glm::translate(glm::mat4(1.f), _vec);
	mats[_idx] = m * mats[_idx];
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

void CShape::setMatrix(CShape& _other)
{
	mats = _other.mats;
}

// --
// CBrick member function
// --

CBrick::CBrick()
{
	setData(SHAPE_DICE);
	setColor(0.f, 1.f, 0.f);
}

CBrick::CBrick(const glm::ivec3& _pos)
{
	setData(SHAPE_DICE);
	setColor(0.f, 1.f, 0.f);
	pos = _pos;
}

void CBrick::draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{

	translate(static_cast<int>(mats.size()), static_cast<glm::vec3>(pos));
	CShape::draw(_program, _view, _proj, _mode, _light);
	mats.pop_back();
}

const glm::ivec3& CBrick::getPos()
{
	return pos;
}

void CMap::init(const int _idx)
{

	std::ifstream inputFile;
	bricks.clear();
	bricks.reserve(150);
	switch (_idx)
	{
	case 0:
		inputFile.open("coordinates\\map0.txt");
		break;
	case 1:
		inputFile.open("coordinates\\map1.txt");
		break;
	case 2:
		inputFile.open("coordinates\\map2.txt");
		break;
	default:
		std::cerr << "CMap()::init(): invaild index." << std::endl;
		return;
	}

	if (!inputFile.is_open()) {
		std::cerr << "CMap()::init(): cannot open file." << std::endl;
		return;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		if (line[0] == '#')
			break;
		int pos[3] = {};
		sscanf_s(line.c_str(), "%d, %d, %d,", &pos[0], &pos[1], &pos[2]);
		bricks.push_back(CBrick(glm::ivec3(pos[1], pos[0], pos[2])));
	}

	inputFile.close();

}

void CMap::updateBuffer()
{
	for (auto& brick : bricks)
		brick.updateBuffer();
}

void CMap::draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	for (auto& brick : bricks)
		brick.draw(_program, _view, _proj, _mode, _light);
}

CBrick& CMap::operator()(const glm::ivec3 _pos)
{
	if (bricks.empty()) {
		std::cerr << "CMap::operator[](): Cannot find any Bricks." << std::endl;
		assert(false);
	}

	
	for (auto& brick : bricks) {
		if (brick.getPos() == _pos) {
			return brick;
		}
	}

	
	std::cerr << "CMap::operator[](): Cannot find Brick. Returned first instance." << std::endl;
	return bricks[0];
}

CBrick& CMap::operator()(const int _y, const int _x, const int _z)
{
	return (*this)(glm::ivec3(_x, _y, _z));
}

void CPlayer::init()
{
	shapes[PLAYER_STAND].setData(SHAPE_PLAYER_STAND);
	shapes[PLAYER_STAND].setColor(0.2f, 0.2f, 0.2f);
	shapes[PLAYER_STAND].updateBuffer();

	shapes[PLAYER_HOLD].setData(SHAPE_PLAYER_HOLD);
	shapes[PLAYER_HOLD].setColor(0.2f, 0.2f, 0.2f);
	shapes[PLAYER_HOLD].updateBuffer();

	shapes[PLAYER_HANG].setData(SHAPE_PLAYER_HANG);
	shapes[PLAYER_HANG].setColor(0.2f, 0.2f, 0.2f);
	shapes[PLAYER_HANG].updateBuffer();
}

void CPlayer::draw(const unsigned int _program, const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	shapes[status].draw(_program, _view, _proj, _mode, _light);
}

void CPlayer::changeStatus(const int _status)
{
	shapes[_status].setMatrix(shapes[status]);
	status = _status;
}

CShape& CPlayer::getShape()
{
	return shapes[status];
}
