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


	// --
	// process member variable
	// --

	std::vector<CShape> shapes;
	
	
	int mode_c = 0;
	int mode_r = 0;

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

		light.pos = { 0.f, 0.f, 5.f };


		shapes.push_back(CShape());
		shapes[0].setData(SHAPE_SPHERE);
		shapes[0].setColor(1.f, 0.f, 0.f);
		shapes[0].translate(0, 0.f, -0.05f, 0.f);
		shapes[0].scale(1, 0.392837782f, 0.392837782f, 0.392837782f);

		shapes.push_back(CShape());
		shapes[1].setData(SHAPE_SPHERE);
		shapes[1].setColor(0.f, 1.f, 0.f);
		shapes[1].translate(0, 0.f, -0.05f, 0.f);
		shapes[1].scale(1, 0.392837782f, 0.392837782f, 0.392837782f);
		shapes[1].scale(1, 0.5f, 0.5f, 0.5f);
		shapes[1].translate(2, -2.0f, 0.f, 0.f);

		shapes.push_back(CShape());
		shapes[2].setData(SHAPE_SPHERE);
		shapes[2].setColor(0.f, 0.f, 1.f);
		shapes[2].translate(0, 0.f, -0.05f, 0.f);
		shapes[2].scale(1, 0.392837782f, 0.392837782f, 0.392837782f);
		shapes[2].scale(1, 0.3f, 0.3f, 0.3f);
		shapes[2].translate(2, -4.0f, 0.f, 0.f);

		shapes.push_back(CShape());
		shapes[3].setData(SHAPE_DICE);
		shapes[3].setColor(0.3f, 0.3f, 0.3f);
		shapes[3].scale(0, 0.3f, 0.3f, 0.3f);
		shapes[3].translate(1, 0.0f, 0.f, 5.f);
		

		for (auto& shape : shapes)
			shape.updateBuffer();

		//for (auto& line : lines)
		//	line.updateBuffer();


		// --
		// set view
		// --

		view.eye = glm::vec3(0.0f, 0.0f, 10.0f);
		view.at = glm::vec3(0.f, 0.f, 0.0f);
		view.up = glm::vec3(0.f, 1.f, 0.f);

		proj = glm::perspective(glm::radians(60.f), 1.0f, 0.1f, 20.f);


		// --
		// explain
		// --

		std::cout << "c:  조명 색을 다른 색으로 바뀌도록 한다. ( W -> R -> G -> B -> W )" << std::endl;
		std::cout << "r/R:  조명의 위치를 중심의 구의 y축에 대하여 양/음 방향으로 회전한다." << std::endl;
		std::cout << "q: 프로그램 종료" << std::endl;

	}

	// --
	// define cb func
	// --

	void Display() const
	{
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int mode = GL_TRIANGLES;

		glEnable(GL_DEPTH_TEST);

		for (const auto& shape : shapes)
			shape.draw(ShaderMgr.program, view, proj, mode, light);


		glutSwapBuffers();
	}

	void Mouse(const int _button, const int _state, const int _x, const int _y)
	{

	}

	void Keyboard(const unsigned char _key, const int _x, const int _y)
	{
		switch (_key)
		{
		case 'c':
			switch (mode_c)
			{
			case 0:
				mode_c++;
				light.color = { 1.f, 0.f, 0.f };
				break;

			case 1:
				mode_c++;
				light.color = { 0.f, 1.f, 0.f };
				break;

			case 2:
				mode_c++;
				light.color = { 0.f, 0.f, 1.f };
				break;

			case 3:
				mode_c = 0;
				light.color = { 1.f, 1.f, 1.f };
				break;
			}
			break;

		case 'r':
			if (mode_r == 1)
				mode_r = 0;
			else
				mode_r = 1;

			break;

		case 'R':
			if (mode_r == -1)
				mode_r = 0;
			else
				mode_r = -1;
			break;

		case 'q':
			PostQuitMessage(0);
			break;

		default:
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
		if (mode_r != 0) {
			float angle = glm::radians(1.f * -mode_r);
			float newX = light.x * cos(angle) - light.z * sin(angle);
			float newZ = light.x * sin(angle) + light.z * cos(angle);
			light.x = newX;
			light.z = newZ;
			shapes[3].rotate(3, 1.f * mode_r, 0.f, 1.f, 0.f);
		}
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