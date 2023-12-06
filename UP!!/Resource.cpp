#include "Resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned int createTexture(const char* _name)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widthImage, heightImage, numberOfChannel;
	unsigned char* data = stbi_load(_name, &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	return texture;
}

// -----
// CShape member function
// -----

CShape::~CShape()
{}

void CShape::setUniform(const SView& _view, const glm::mat4& _proj, const SLight& _light, const GLuint _program)
{
	unsigned int lightPosLocation = glGetUniformLocation(_program, "lightPos");
	glUniform3f(lightPosLocation, _light.x, _light.y, _light.z);

	unsigned int lightColorLocation = glGetUniformLocation(_program, "lightColor");
	glUniform3f(lightColorLocation, _light.r, _light.g, _light.b);

	unsigned int objColorLocation = glGetUniformLocation(_program, "objectColor");
	glUniform3f(objColorLocation, color.r, color.g, color.b);

	unsigned int viewPosLocation = glGetUniformLocation(_program, "viewPos");
	glUniform3f(viewPosLocation, _view.eye.x, _view.eye.y, _view.eye.z);

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


}

void CShape::drawBuffer(const int _mode)
{
	glBindVertexArray(buffer.VAO);

	if (data->indices.empty()) {
		glDrawArrays(_mode, 0, static_cast<GLsizei>(data->coords.size() / 3));
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
		glDrawElements(_mode, static_cast<GLsizei>(data->indices.size()), GL_UNSIGNED_INT, 0);
	}
}

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

void CShape::updateTextureBuffer()
{
	if (data == nullptr) {
		std::cerr << "data isn't set" << std::endl;
		return;
	}

	if (buffer.VAO != 0) {
		glDeleteVertexArrays(1, &buffer.VAO);
		glDeleteBuffers(3, buffer.VBO);
	}

	glGenBuffers(3, buffer.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, data->coords.size() * sizeof(GLfloat), data->coords.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, data->normals.size() * sizeof(GLfloat), data->normals.data(), GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, data->textures.size() * sizeof(GLfloat), data->textures.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &buffer.VAO);
	glBindVertexArray(buffer.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

void CShape::setData(const int _shape)
{
	data = &CShapeDataMgr::getInst()->getData(_shape);
}

void CShape::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	GLuint _program = CShaderMgr::getInst()->getProgram(SHADER_COMMON_PROGRAM);
	glUseProgram(_program);

	setUniform(_view, _proj, _light, _program);

	drawBuffer(_mode);
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

const size_t CShape::getMatrixSize() const
{
	return mats.size();
}

void CShape::popMatrix()
{
	mats.pop_back();
}

// -----
// CBrick member function
// -----

std::array<unsigned int, BRICK_TYPE_MAX> CBrick::textures = {-1};

CBrick::CBrick()
{
	setData(SHAPE_DICE);
	setColor(1.f, 1.f, 1.f);
	if (textures[0] == -1)
		updateTextures();
}

CBrick::CBrick(const glm::ivec3& _pos)
{
	setData(SHAPE_DICE);
	setColor(1.f, 1.f, 1.f);
	pos = _pos;
	if (textures[0] == -1)
		updateTextures();
}

void CBrick::updateBuffer()
{
	CShape::updateTextureBuffer();
}

void CBrick::updateTextures()
{
	for (int i = 0; i < BRICK_TYPE_MAX; ++i)
		textures[i] = createTexture(BRICK_IMAGE[i]);
}

void CBrick::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{

	translate(static_cast<int>(getMatrixSize()), static_cast<glm::vec3>(pos));
	
	GLuint _program = CShaderMgr::getInst()->getProgram(SHADER_TEXTURE_PROGRAM);
	glUseProgram(_program);

	setUniform(_view, _proj, _light, _program);

	glBindTexture(GL_TEXTURE_2D, textures[type]);

	drawBuffer(_mode);

	popMatrix();
}

const glm::ivec3& CBrick::getPos() const
{
	return pos;
}

void CBrick::setPos(const int _y, const int _x, const int _z)
{
	pos = glm::ivec3(_x, _y, _z);
}

void CBrick::move(const int _dy, const int _dx, const int _dz)
{
	pos.x += _dx;
	pos.y += _dy;
	pos.z += _dz;
}

void CBrick::setType(const int _type)
{
	type = _type;
}

const int CBrick::getType() const
{
	return type;
}


// ------
// CMap member function
// ------

void CMap::init(const int _idx)
{
	if (_idx >= MAP_MAX) {
		std::cerr << "CMap()::init(): invaild index." << std::endl;
		return;
	}

	std::string filename = "coordinates\\map" + std::to_string(_idx) + ".txt";
	std::ifstream inputFile;
	bricks.clear();
	items.clear();
	bricks.reserve(150);

	inputFile.open(filename);
	if (!inputFile.is_open()) {
		std::cerr << "CMap()::init(): cannot open file." << std::endl;
		return;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		if (line[0] == '#')
			break;
		int pos[4] = {};
		if (line[0] == '@') {
			sscanf_s(line.c_str(), "@%d, %d, %d, %d", &pos[0], &pos[1], &pos[2], &pos[3]);
			items.push_back(CItem(glm::ivec3(pos[1], pos[0], pos[2])));
			continue;
		}
		sscanf_s(line.c_str(), "%d, %d, %d, %d", &pos[0], &pos[1], &pos[2], &pos[3]);

		CBrick brick_new;
		brick_new.setPos(pos[0], pos[1], pos[2]);
		if (pos[3] != 0)
			brick_new.setType(pos[3] - 1);
		bricks.push_back(brick_new);
	}

	inputFile.close();
}

void CMap::updateBuffer()
{
	for (auto& brick : bricks)
		brick.updateBuffer();

	for (auto& item : items)
		item.updateBuffer();
}

void CMap::update()
{
	for (auto& item : items)
		item.update();
}

void CMap::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	for (auto& brick : bricks)
		brick.draw(_view, _proj, _mode, _light);
	for (auto& item : items)
		item.draw(_view, _proj, _mode, _light);

}

const bool CMap::createBrick(const int _y, const int _x, const int _z, const int _type)
{
	if (isPosition(_y, _x, _z))
		return false;
	CBrick brick_new;
	brick_new.setPos(_y, _x, _z);
	brick_new.setType(_type);
	brick_new.updateBuffer();
	bricks.push_back(brick_new);
	return true;
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

const bool CMap::isPosition(const int _y, const int _x, const int _z) const
{
	for (auto& brick : bricks)
		if (brick.getPos() == glm::ivec3(_x, _y, _z))
			return true;
	return false;
}

const bool CMap::isPositionItem(const int _y, const int _x, const int _z) const
{
	for (const auto& item : items) {
		if (item.getPos() == glm::ivec3(_x, _y, _z)) {
			return true;
		}
	}
	return false;
}

void CMap::deleteItem(const int _y, const int _x, const int _z)
{
	for (std::list<CItem>::iterator iter = items.begin(); iter != items.end(); ++iter) {
		if (iter->getPos() == glm::ivec3(_x, _y, _z)) {
			items.erase(iter);
			return;
		}
	}
}

// -----
// CPlayer member function
// -----

void CPlayer::updateBuffer()
{
	glm::vec3 color(PLAYER_COLOR_NORMAL);
	shapes[PLAYER_STAND].setData(SHAPE_PLAYER_STAND);
	shapes[PLAYER_STAND].setColor(color);
	shapes[PLAYER_STAND].updateBuffer();

	shapes[PLAYER_HOLD].setData(SHAPE_PLAYER_HOLD);
	shapes[PLAYER_HOLD].setColor(color);
	shapes[PLAYER_HOLD].updateBuffer();

	shapes[PLAYER_HANG].setData(SHAPE_PLAYER_HANG);
	shapes[PLAYER_HANG].setColor(color);
	shapes[PLAYER_HANG].updateBuffer();

	shapes[PLAYER_MOVING].setData(SHAPE_PLAYER_MOVING);
	shapes[PLAYER_MOVING].setColor(color);
	shapes[PLAYER_MOVING].updateBuffer();
}

void CPlayer::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	shapes[status].draw(_view, _proj, _mode, _light);
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

int& CPlayer::getstatus()
{
	return status;
}

void CPlayer::setColor(const float _r, const float _g, const float _b)
{
	for (auto& shape : shapes)
		shape.setColor(_r, _g, _b);
}

void CPlayer::setColor(const glm::vec3& _color)
{
	for (auto& shape : shapes)
		shape.setColor(_color);
}


// -----
// CBrick member function
// -----

CBackground::~CBackground()
{
	glDeleteTextures(1, &texture);
}

void CBackground::updateBuffer()
{
	texture = createTexture(BACKGROUND_IMAGE);
	setData(SHAPE_SQUARE);
	setColor(1.f, 1.f, 1.f);
	scale(0, 110.f, 110.f, 110.f);
	rotate(0, 90.f, 1.f, 0.f, 0.f);
	translate(0, 0.f, 0.f, -50.f);
	CShape::updateTextureBuffer();
}

void CBackground::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	GLuint _program = CShaderMgr::getInst()->getProgram(SHADER_TEXTURE_PROGRAM);
	glUseProgram(_program);

	setUniform(_view, _proj, _light, _program);

	glBindTexture(GL_TEXTURE_2D, texture);

	drawBuffer(_mode);
}

// -----
// CLava member function
// -----

void CLava::updateBuffer()
{
	for (int i = 1; i < 8; ++i) {
		for (int j = 0; j < 7; ++j) {
			int idx = (i - 1) * 7 + j;
			float x =
				i % 2 ?
				(j - 3) * 2.f :
				(j - 3) * 2.f + 1.f;
			float z = (i - 4) * 2.f;
			shapes[idx].setData(SHAPE_SPHERE);
			shapes[idx].setColor(0.7f, 0.f, 0.f);
			shapes[idx].translate(0, 0.f, -0.05f, 0.f);
			shapes[idx].scale(0, 0.392837782f, 0.392837782f, 0.392837782f);
			shapes[idx].scale(0, 2.f, 2.f, 2.f);
			shapes[idx].translate(0, x, 0.f, z);
			shapes[idx].updateBuffer();
		}
	}
}

void CLava::reset()
{
	pos = { 0.f, -10.f, 0.f };
	speed = 0;
}


void CLava::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	for (auto& shape : shapes) {
		shape.translate(shape.getMatrixSize(), pos);
		shape.draw(_view, _proj, _mode, _light);
		shape.popMatrix();
	}
}

void CLava::move(const float _dx, const float _dz)
{
	pos.x += _dx;
	pos.z += _dz;
}

const int CLava::getY() const
{
	return static_cast<int>(floorf(pos.y));
}

void CLava::setSpeed(const int _speed)
{
	speed = _speed;
}

void CLava::update()
{
	pos.y += 0.001f * static_cast<float>(speed);
}

CItem::CItem(const glm::ivec3& _pos)
{
	pos = _pos;
}

void CItem::updateBuffer()
{
	setData(SHAPE_SPHERE);
	setColor(1.f, 0.5f, 0.f);
	translate(0, 0.f, -0.05f, 0.f);
	scale(0, 0.392837782f, 0.392837782f, 0.392837782f);
	scale(0, 0.25f, 0.25f, 0.25f);
	CShape::updateBuffer();
}

void CItem::draw(const SView& _view, const glm::mat4& _proj, const int _mode, const SLight& _light)
{
	translate(getMatrixSize(), static_cast<float>(pos.x), static_cast<float>(pos.y + 1), static_cast<float>(pos.z));
	CShape::draw(_view, _proj, _mode, _light);
	popMatrix();
}

const glm::ivec3& CItem::getPos() const
{
	return pos;
}

void CItem::update()
{
	size_time--;
	if (size_dir) {
		scale(1, 0.99f, 0.99f, 0.99f);
	}
	else {
		float value = 100.f / 99.f;
		scale(1, value, value, value);
	}

	if (size_time <= 0) {
		size_time = 100;
		size_dir = !size_dir;
	}
}