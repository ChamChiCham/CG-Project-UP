#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include "CShaderMgr.h"
#include "CShapeDataMgr.h"
#include "Define.h"
#include "Struct.h"
#include "Resource.h"


// --
// declare callback function 
// --

namespace cham
{
	GLvoid Display();
	GLvoid Reshape(int w, int h);
	GLvoid GameLoop(int value);
	GLvoid Mouse(int button, int state, int x, int y);
	GLvoid Keyboard(unsigned char key, int x, int y);
	GLvoid Motion(int x, int y);
}




class CWindowMgr
{
private:

	// singleton
	static CWindowMgr* instance;

	CWindowMgr()
	{}
	~CWindowMgr()
	{
		if (!instance) delete instance;
	}

public:

	CWindowMgr(const CWindowMgr& other) = delete;
	CWindowMgr& operator=(const CWindowMgr& other) = delete;

	static CWindowMgr* getInst()
	{
		if (!instance) {
			instance = new CWindowMgr();
		}
		return instance;
	}


private:

	// Manager
	CShaderMgr ShaderMgr;

	// default member variable
	SView		view;
	SLight		light;
	glm::mat4	proj = glm::mat4(1.f);
	glm::vec3	background = glm::vec3(0.f, 0.f, 0.f);


	// ---
	// process member variable
	// ---

	std::vector<CShape> shapes;

	CMap map;

	float d = 0.f;
	float s = 0.f;
public:

	// --
	// Basic func
	// --

	void init(int& argc, char** argv)
	{
		// init GLUT
		glutInit(&argc, argv);
		glutInitDisplayMode(WINDOW_DISPLAYMODE);
		glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);
		glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
		glutCreateWindow(WINDOW_TITLE);

		// init GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "Unable to initialize GLEW" << std::endl;
			exit(EXIT_FAILURE);
		}
		else
			std::cout << "GLEW Initialized\n";

		ShaderMgr.makeProgram("Vertex.glsl", "Fragment.glsl");


		// set cb func
		glutDisplayFunc(cham::Display);
		glutReshapeFunc(cham::Reshape);
		glutMouseFunc(cham::Mouse);
		glutKeyboardFunc(cham::Keyboard);
		glutTimerFunc(10, cham::GameLoop, 1);
		glutMotionFunc(cham::Motion);


		// --
		// create shape data
		// --

		// 조명의 위치.
		light.pos = { 3.f, 0.f, 10.f };
		
		// #####
		// readme
		// #####
		// brick에 setData와 setColor을 할 필요가 없다. 기본적으로 SHAPE_DICE 형태와 0x00FF00의 색이 들어가 있다.
		// 색을 바꾸고 싶다면 다른 색으로 setColor을 해주어도 무방하다.
		// brick의 좌표는 glm::vec3 형태의 pos 또는 float x,y,z 로 설정할 수 있다. 각 좌표가 블록의 원점이라고 생각하면 편하다.

		// 하지만 마지막에 업데이트 버퍼는 해주어야 한다.
		// brick.updateBuffer();

		// 아래는 맵을 만드는 과정이다.
		// brick_data에 float형 좌표를 넣어주면 된다.
		// 순서는 y, x, z순이다.
		std::vector<int> brick_data =
		{
			0, 0, 0,
			0, 1, 0,
			0, 2, 0,
			0, 3, 0,
			0, 4, 0,
			0, 5, 0,

			1, 0, -1,
			1, 1, -1,
			1, 2, -1,
			1, 3, -1,
			1, 4, -1,
			1, 5, -1,

			2, 0, -2,
			2, 1, -2,
			2, 2, -2,
			2, 3, -2,
			2, 4, -2,
			2, 5, -2,

			3, 1, -2,
			3, 4, -2,

			3, 0, -3,
			3, 2, -3,
			3, 3, -3,
			3, 5, -3,

			4, 0, -3,
			4, 1, -3,
			4, 2, -3,
			4, 3, -3,
			4, 4, -3,
			4, 5, -3,

			5, 1, -3,
			5, 4, -3,

			5, 0, -4,
			5, 2, -4,
			5, 3, -4,
			5, 5, -4,

			6, 0, -4,
			6, 1, -4,
			6, 2, -4,
			6, 3, -4,
			6, 4, -4,
			6, 5, -4,

			7, 1, -4,
			7, 3, -4,
			7, 4, -4,

			7, 0, -5, 
			7, 1, -5,
			7, 2, -5,
			7, 3, -5,
			7, 4, -5,
			7, 5, -5,

			8, 1, -5,
			8, 2, -5,
			8, 3, -5,
			8, 4, -5,
			8, 5, -5,

			9, 0, -5,
			9, 1, -5,
			9, 3, -5,
			9, 4, -5,

			9, 0, -6,
			9, 1, -6,
			9, 2, -6,
			9, 3, -6,
			9, 4, -6,
			9, 5, -6,

			10, 1, -6,
			10, 2, -6,
			10, 3, -6,
			10, 4, -6,
			10, 5, -6,

			11, 0, -6,
			11, 1, -6,
			11, 2, -6,
			11, 3, -6,
			11, 4, -6,
			11, 5, -6,

			12, 0, -7,
			12, 1, -7,
			12, 2, -7,
			12, 3, -7,
			12, 4, -7,
			12, 5, -7,

			13, 0, -7,
			13, 1, -7,
			13, 2, -7,
			13, 3, -7,
			13, 4, -7,
			13, 5, -7,

			14, 0, -8,
			14, 1, -8,
			14, 2, -8,
			14, 3, -8,
			14, 4, -8,
			14, 5, -8,

			15, 0, -8,
			15, 1, -8,
			15, 2, -8,
			15, 3, -8,
			15, 4, -8,

			16, 2, -8,
			16, 3, -8,
			16, 4, -8,
			16, 5, -8,

			16, 0, -9,
			16, 1, -9,
			16, 2, -9,
			16, 3, -9,
			16, 4, -9,
			16, 5, -9,

			17, -1, -9,
			17, 0, -9,
			17, 1, -9,
			17, 2, -9,
			17, 3, -9,
			17, 4, -9,
			17, 5, -9,

			18, -2, -9,
			18, 0, -9,
			18, 2, -9,
			18, 3, -9,
			18, 4, -9,
			18, 5, -9,

			18, 0, -10,
			18, 1, -10,
			18, 2, -10,
			18, 3, -10,
			18, 4, -10,
			18, 5, -10,

			19, 0, -9,
			19, 1, -9,
			19, 2, -9,
			19, 3, -9,

			19, 0, -10,
			19, 1, -10,
			19, 2, -10,
			19, 3, -10,
			19, 4, -10,

			20, 0, -11,
			20, 1, -11,
			20, 2, -11,
			20, 3, -11,
			20, 4, -11,
			20, 5, -11,

			21, 0, -11,
			21, 1, -11,
			21, 2, -11,
			21, 3, -11,
			21, 4, -11,

			22, 3, -11,
		};

		map.createBricks(brick_data);

		for (auto& shape : shapes)
			shape.updateBuffer();

		//for (auto& line : lines)
		//	line.updateBuffer();

		for (auto& brick : map.bricks)
			brick.updateBuffer();

		// ---
		// READ ME: use this
		// ---
		map(7, 1, -4).setColor(0.5f, 0.5f, 0.5f);
		map(22, 3, -11).setColor(1.f, 0.f, 0.f);
		
		// --
		// set view
		// --

		view.eye = glm::vec3(3.0f, 3.0f, 10.0f);
		view.at = glm::vec3(3.f, 3.f, 0.0f);
		view.up = glm::vec3(0.f, 1.f, 0.f);

		proj = glm::perspective(glm::radians(60.f), static_cast<float>(WINDOW_SIZE_X) / static_cast<float>(WINDOW_SIZE_Y), 0.1f, 20.f);


		// --
		// explain
		// --

		std::cout << "4/6: 카메라 x이동" << std::endl;
		std::cout << "8/5: 카메라 y이동" << std::endl;
		std::cout << "1/3: 카메라 z이동" << std::endl;
		std::cout << "7/9: 카메라 y회전" << std::endl;
	}

	// --
	// define cb func
	// --

	void Display()
	{
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int mode = GL_TRIANGLES;

		glEnable(GL_DEPTH_TEST);

		for (auto& shape : shapes)
			shape.draw(ShaderMgr.program, view, proj, mode, light);

		// 맵에 있는 brick 그리기
		for (auto& brick : map.bricks)
			brick.draw(ShaderMgr.program, view, proj, mode, light);


		glutSwapBuffers();
	}

	void Mouse(const int _button, const int _state, const int _x, const int _y)
	{

	}

	void Keyboard(const unsigned char _key, const int _x, const int _y)
	{
		switch (_key) {
		case '8':
			view.eye.y += 0.1;
			view.at.y += 0.1;
			break;
		case '5':
			view.eye.y -= 0.1;
			view.at.y -= 0.1;
			break;
		case '4':
			view.eye.x -= 0.1;
			view.at.x -= 0.1;
			break;
		case '6':
			view.eye.x += 0.1;
			view.at.x += 0.1;
			break;
		case '3':
			view.eye.z += 0.1;
			view.at.z += 0.1;
			break;
		case '1':
			view.eye.z -= 0.1;
			view.at.z -= 0.1;
			break;
		case '7':
			d = sqrtf(powf(view.eye.x - view.at.x, 2) + powf(view.eye.z - view.at.z, 2));
			s = atan2f(view.eye.z - view.at.z, view.eye.x - view.at.x);
			view.eye.x = view.at.x + d * cos(s + glm::radians(2.f));
			view.eye.z = view.at.z + d * sin(s + glm::radians(2.f));
			break;
		case '9':
			d = sqrtf(powf(view.eye.x - view.at.x, 2) + powf(view.eye.z - view.at.z, 2));
			s = atan2f(view.eye.z - view.at.z, view.eye.x - view.at.x);
			view.eye.x = view.at.x + d * cos(s + glm::radians(-2.f));
			view.eye.z = view.at.z + d * sin(s + glm::radians(-2.f));
			break;
		}
	}

	void Motion(const int _x, const int _y)
	{
		
	}

	// --
	// process func
	// --


	void updateState()
	{
		
	}


	void run()
	{
		glutMainLoop();
	}

};

CWindowMgr* CWindowMgr::instance = nullptr;


// (CALLBACK) Display screen
GLvoid cham::Display()
{
	CWindowMgr::getInst()->Display();
}

// (CALLBACK) Reset Viewport
GLvoid cham::Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

// (CALLBACK) Mouse click event
GLvoid cham::Mouse(int button, int state, int x, int y)
{
	CWindowMgr::getInst()->Mouse(button, state, x, y);
}

GLvoid cham::Keyboard(unsigned char key, int x, int y)
{
	CWindowMgr::getInst()->Keyboard(key, x, y);
}

GLvoid cham::Motion(int x, int y)
{
	CWindowMgr::getInst()->Motion(x, y);
}

// (CALLBACK) Main Loop
// FPS : 100
GLvoid cham::GameLoop(int value)
{

	// Game State update
	CWindowMgr::getInst()->updateState();

	// render (Display 함수 호출)
	glutPostRedisplay();

	glutTimerFunc(10, GameLoop, 1);
}

int main(int argc, char** argv)
{
	CWindowMgr::getInst()->init(argc, argv);
	CWindowMgr::getInst()->run();
}