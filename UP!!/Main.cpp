#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <array>

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
	GLvoid SpecialKeys(int key, int x, int y);
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

	// default member variable
	SView		view;
	SLight		light;
	glm::mat4	proj = glm::mat4(1.f);
	glm::vec3	background = glm::vec3(0.f, 0.f, 0.f);


	// ---
	// process member variable
	// ---

	std::vector<CShape> shapes;

	CPlayer				player;
	CMap*				map_ptr = nullptr;
	std::array<CMap, 3> maps;
	
	// 전역 변수
	float d = 0.f;
	float s = 0.f;
	int current_map;
	int moving_time;
	bool moving_left;
	bool moving_right;
	bool moving_up;
	bool moving_down;

	typedef struct CurrentPos {
		int xPos;
		int yPos;
		int zPos;
		int way;
	};
	CurrentPos playerPos;
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

		CShaderMgr::getInst()->init();


		// set cb func
		glutDisplayFunc(cham::Display);
		glutReshapeFunc(cham::Reshape);
		glutMouseFunc(cham::Mouse);
		glutKeyboardFunc(cham::Keyboard);
		glutTimerFunc(10, cham::GameLoop, 1);
		glutMotionFunc(cham::Motion);
		glutSpecialFunc(cham::SpecialKeys);

		// --
		// create shape data
		// --
		

		for (int i = 0; i < 3; ++i)
			maps[i].init(i);

		map_ptr = &maps[current_map];

		player.init();

		for (auto& shape : shapes)
			shape.updateBuffer();

		for (auto& map : maps)
			map.updateBuffer();

		// map(21, 6, -8).setColor(1.f, 0.f, 0.f);
		
		// --
		// set basic variable
		// --
		light.pos = { 3.f, 0.f, 10.f };

		view.eye = glm::vec3(3.0f, 2.0f, 5.0f);
		view.at = glm::vec3(3.f, 2.f, 0.0f);
		view.up = glm::vec3(0.f, 1.f, 0.f);

		// 초기 플레이어 위치/크기 조정
		player.getShape().scale(0, 0.015f, 0.015f, 0.015f);
		player.getShape().translate(1, 0.f, 0.5f, 0.1f);
		player.getShape().rotate(2, 180.f, 0.f, 1.f, 0.f);
		playerPos = { 0, 0, 0 };

		proj = glm::perspective(glm::radians(60.f), static_cast<float>(WINDOW_SIZE_X) / static_cast<float>(WINDOW_SIZE_Y), 0.1f, 20.f);

		// 초기 카메라 위치 조정
		d = sqrtf(powf(view.eye.x - view.at.x, 2) + powf(view.eye.z - view.at.z, 2));
		s = atan2f(view.eye.z - view.at.z, view.eye.x - view.at.x);
		view.eye.x = view.at.x + d * cos(s + glm::radians(-20.f));
		view.eye.z = view.at.z + d * sin(s + glm::radians(-20.f));

		// --
		// explain
		// --

		std::cout << "방향키: 플레이어 이동" << std::endl;
		std::cout << "b: 스탠드 자세" << std::endl;
		std::cout << "n: 홀드 자세" << std::endl;
		std::cout << "m: 행 자세" << std::endl;
		std::cout << "4/6: 카메라 x이동" << std::endl;
		std::cout << "8/5: 카메라 y이동" << std::endl;
		std::cout << "1/3: 카메라 z이동" << std::endl;
		std::cout << "7/9: 카메라 y회전" << std::endl;
		std::cout << "=/-: 스테이지 강제 변환" << std::endl;
	}

	// --
	// process function
	// --


	// --
	// define cb func
	// --
	enum {front, right, back, left};

	void Display()
	{
		glClearColor(background.r, background.g, background.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int mode = GL_TRIANGLES;

		glEnable(GL_DEPTH_TEST);

		for (auto& shape : shapes)
			shape.draw(view, proj, mode, light);

		// 맵에 있는 brick 그리기
		map_ptr->draw(view, proj, mode, light);

		player.draw(view, proj, mode, light);

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
		case '=':
			if (current_map < 2) {
				current_map++;
				map_ptr = &maps[current_map];
				std::cout << "Stage Change" << std::endl;
				std::cout << "Current Map: " << current_map << std::endl;
			}
			break;
		case '-':
			if (current_map > 0) {
				current_map--;
				map_ptr = &maps[current_map];
				std::cout << "Stage Change" << std::endl;
				std::cout << "Current Map: " << current_map << std::endl;
				break;
			}
		case 'b':
			player.changeStatus(PLAYER_STAND);
			break;
		case 'n':
			player.changeStatus(PLAYER_HOLD);
			break;
		case 'm':
			player.changeStatus(PLAYER_HANG);
			break;
		case ']':
			if (maps[0].isPosition(0, 0, 0))
				std::cout << "True" << std::endl;
			else
				std::cout << "False" << std::endl;	
			break;
		}
	}

	void Motion(const int _x, const int _y)
	{
		
	}

	void SpecialKeys(const int _key, const int _x, const int _y)
	{
		switch (_key) {
		case GLUT_KEY_UP:
			// 이동중이 아니거나, 벤 조건에 만족하지 않으면 이동
			if (moving_time == 0 && MoveUpBan() == false) {
				player.changeStatus(PLAYER_HANG);
				moving_up = true;
				playerPos.yPos += 1;
				playerPos.zPos -= 1;
			}
			// 이전에 바라보던 방향에 맞게 현재 바라보는 방향 수정
			if (playerPos.way == right) {
				player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
			}
			else if (playerPos.way == left) {
				player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
			}
			// 현재 바라보는 방향 업데이트
			playerPos.way = front;
			break;
		case GLUT_KEY_DOWN:
			if (moving_time == 0 && MoveDownBan() == false) {
				player.changeStatus(PLAYER_HANG);
				moving_down = true;
				playerPos.yPos -= 1;
				playerPos.zPos += 1;
			}
			if (playerPos.way == right) {
				player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
			}
			else if (playerPos.way == left) {
				player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
			}
			playerPos.way = back;
			break;
		case GLUT_KEY_LEFT:
			if (moving_time == 0 && MoveLeftBan() == false) {
				moving_left = true;
				playerPos.xPos -= 1;
			}
			if (playerPos.way == front || playerPos.way == back) {
				player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
			}
			else if (playerPos.way == right) {
				player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
			}
			playerPos.way = left;
			break;
		case GLUT_KEY_RIGHT:
			if (moving_time == 0 && MoveRightBan() == false) {
				moving_right = true;
				playerPos.xPos += 1;
			}
			if (playerPos.way == front || playerPos.way == back) {
				player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
			}
			else if (playerPos.way == left) {
				player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
			}
			playerPos.way = right;
			break;
		}
		std::cout << "xPos:" << playerPos.xPos << std::endl;
		std::cout << "yPos:" << playerPos.yPos << std::endl;
		std::cout << "zPos:" << playerPos.zPos << std::endl;
		if (playerPos.way == 0) {
			std::cout << "way: front" << std::endl;
		}
		else if (playerPos.way == 1) {
			std::cout << "way: right" << std::endl;
		}
		else if (playerPos.way == 2) {
			std::cout << "way: back" << std::endl;
		}
		else if (playerPos.way == 3) {
			std::cout << "way: left" << std::endl;
		}
	}

	void MovingUp()
	{
		if (moving_up) {
			player.getShape().translate(3, 0.f, 0.1f, 0.f);
			player.getShape().translate(3, 0.f, 0.f, -0.1f);
			view.eye.y += 0.1f;
			view.at.y += 0.1f;
			EndMove();
		}
	}

	void MovingDown()
	{
		if (moving_down) {
			player.getShape().translate(3, 0.f, -0.1f, 0.f);
			player.getShape().translate(3, 0.f, 0.f, 0.1f);
			view.eye.y -= 0.1f;
			view.at.y -= 0.1f;
			EndMove();
		}
	}

	void MovingLeft()
	{
		if (moving_left) {
			player.getShape().translate(3, -0.1f, 0.f, 0.f);
			EndMove();
		}
	}

	void MovingRight()
	{
		if (moving_right) {
			player.getShape().translate(3, 0.1f, 0.f, 0.f);
			EndMove();
		}
	}

	void EndMove()
	{
		moving_time++;
		// 무빙 종료 조건
		if (moving_time == 10) {
			player.changeStatus(PLAYER_STAND);
			moving_time = 0;
			moving_right = false;
			moving_left = false;
			moving_up = false;
			moving_down = false;
		}
	}

	// 어떠한 조건을 충족하면 이동을 막음
	bool MoveUpBan()
	{
		if (playerPos.way == right || playerPos.way == left) {
			return true;
		}
		return false;
	}

	bool MoveDownBan()
	{
		if (playerPos.yPos == 0) {
			return true;
		}
		return false;
	}

	bool MoveLeftBan()
	{
		return false;
	}

	bool MoveRightBan()
	{
		return false;
	}

	// --
	// process func
	// --


	void updateState()
	{
		MovingUp();
		MovingDown();
		MovingLeft();
		MovingRight();
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

GLvoid cham::SpecialKeys(int key, int x, int y)
{
	CWindowMgr::getInst()->SpecialKeys(key, x, y);
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