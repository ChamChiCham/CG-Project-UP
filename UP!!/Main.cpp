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
		

		std::vector<int> brick_data =
		{
			0, 0, 0,
			0, 1, 0,
			0, 2, 0,
			0, 3, 0,

			1, 2, 0,
			1, 3, 0,

			1, 0, -1,
			1, 1, -1,
			1, 2, -1,
			1, 3, -1,

			2, 1, -1,
			2, 2, -1,
			2, 3, -1,

			3, 0, -1,
			3, 1, -1,
			3, 2, -1,
			3, 3, -1,

			4, 0, -2,
			4, 1, -2,
			4, 2, -2,
			4, 3, -2,

			5, 0, -2,
			5, 1, -2,
			5, 2, -2,
			5, 3, -2,
			5, 4, -2,

			6, 1, -2,
			6, 2, -2,
			6, 3, -2,
			6, 4, -2,
			6, 5, -2,

			7, 2, -2,
			7, 3, -2,
			7, 4, -2,
			7, 5, -2,
			7, 6, -2,

			8, 4, -2,
			8, 5, -2,

			9, 4, -2,

			8, 2, -3,
			8, 3, -3,
			8, 4, -3,
			8, 5, -3,
			8, 6, -3,

			9, 2, -3,
			9, 3, -3,
			9, 4, -3,
			9, 5, -3,
			9, 6, -3,

			10, 2, -3,
			10, 3, -3,
			10, 4, -3,
			10, 5, -3,
			10, 6, -3,

			11, 1, -3,
			11, 2, -3,
			11, 3, -3,

			11, 1, -4,
			11, 2, -4,
			11, 3, -4,
			11, 4, -4,

			12, 0, -3,
			12, 1, -3,
			12, 2, -3,

			12, 1, -4,
			12, 2, -4,
			12, 3, -4,
			12, 4, -4,

			13, 0, -4,
			13, 1, -4,
			13, 2, -4,
			13, 3, -4,
			13, 4, -4,

			14, 0, -4,
			14, 1, -4,
			14, 2, -4,

			15, 0, -5,
			15, 1, -5,
			15, 2, -5,

			16, -1, -6,
			16, 0, -6,
			16, 1, -6,

			17, -2, -6,
			17, -1, -6,
			17, 0, -6,

			17, -2, -7,
			17, -1, -7,
			17, 0, -7,
			17, 1, -7,

			18, -2, -7,
			18, -1, -7,
			18, 0, -7,
			18, 1, -7,

			19, -2, -7,
			19, -1, -7,
			19, 1, -7,

			20, -2, -7,
			20, -1, -7,
			20, 0, -7,
			20, 1, -7,
			20, 2, -7,

			21, -2, -8,
			21, -1, -8,
			21, 0, -8,
			21, 1, -8,

			22, -2, -8,
			22, -1, -8,
			22, 0, -8,
			22, 1, -8,

			23, -2, -8,
			23, -1, -8,
			23, 1, -8,
			23, 2, -8,

			24, -1, -8,
			24, 0, -8,
			24, 1, -8,
			24, 2, -8,
			24, 3, -8,

			25, 0, -8,
			25, 1, -8,
			25, 2, -8,
			25, 3, -8,

			26, 4, -8,

			26, 1, -9,
			26, 2, -9,
			26, 3, -9,
			26, 4, -9,

			27, 5, -8,

			27, 1, -9,
			27, 2, -9,
			27, 3, -9,
			27, 4, -9,
			27, 5, -9,

			28, 6, -8,

			28, 1, -9,
			28, 2, -9,
			28, 3, -9,
			28, 4, -9,
			28, 5, -9,
			28, 6, -9,

			29, 3, -9,
			29, 4, -9,
			29, 5, -9,
			29, 6, -9,

			30, 3, -10,
			30, 4, -10,
			30, 5, -10,
			30, 6, -10,

			31, 2, -10,
			31, 3, -10,
			31, 4, -10,
			31, 5, -10,
			31, 6, -10,

			32, 2, -10,
			32, 3, -10,
			32, 4, -10,
			32, 5, -10,
			32, 6, -10,

			33, 2, -11,
			33, 3, -11,
			33, 4, -11,
			33, 5, -11,
			33, 6, -11,

			34, 2, -11,
			34, 3, -11,
			34, 4, -11,

			35, 2, -11,
			35, 3, -11,

			36, 2, -11,
		};

		map.createBricks(brick_data);

		for (auto& shape : shapes)
			shape.updateBuffer();

		//for (auto& line : lines)
		//	line.updateBuffer();

		for (auto& brick : map.bricks)
			brick.updateBuffer();

		// map(21, 6, -8).setColor(1.f, 0.f, 0.f);
		
		// --
		// set view
		// --
		light.pos = { 3.f, 0.f, 10.f };

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