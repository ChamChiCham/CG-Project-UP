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
	glm::vec3	background_color = glm::vec3(0.f, 0.f, 0.f);


	// ---
	// process member variable
	// ---

	std::vector<CShape> shapes;

	CPlayer				player;
	CMap*				map_ptr = nullptr;
	std::array<CMap, 3> maps;
	CBackground			background_image;
	
	// 전역 변수
	float d = 0.f;
	float s = 0.f;
	int current_map;
	int moving_time;

	bool moving_left;
	bool moving_right;
	bool moving_front;
	bool moving_back;

	bool moving_left_up;
	bool moving_right_up;
	bool moving_front_up;
	bool moving_back_up;

	bool moving_left_down;
	bool moving_right_down;
	bool moving_front_down;
	bool moving_back_down;

	bool moving_left_hold;
	bool moving_right_hold;
	bool moving_front_hold;
	bool moving_back_hold;

	int bottom_adjustment;

	int movewait_brick_xPos;
	int movewait_brick_yPos;
	int movewait_brick_zPos;

	typedef struct CurrentState {
		int xPos;
		int yPos;
		int zPos;
		int way;
		bool hold;
		bool hard;
		int hold_brick_xPos;
		int hold_brick_yPos;
		int hold_brick_zPos;
	};
	CurrentState playerstate;
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

		player.updateBuffer();

		background_image.updateBuffer();

		for (auto& map : maps)
			map.updateBuffer();

		// map(21, 6, -8).setColor(1.f, 0.f, 0.f);
		
		// --
		// set basic variable
		// --
		// 초기 조명 위치
		light.pos = { 3.f, 70.f, 10.f };

		// 초기 플레이어 위치/크기 조정
		player.getShape().scale(0, 0.015f, 0.015f, 0.015f);
		player.getShape().translate(1, 0.f, 0.5f, 0.1f);
		player.getShape().rotate(2, 180.f, 0.f, 1.f, 0.f);
		playerstate = { 0, 0, 0 };

		proj = glm::perspective(glm::radians(60.f), static_cast<float>(WINDOW_SIZE_X) / static_cast<float>(WINDOW_SIZE_Y), 0.1f, 100.f);

		// 초기 카메라 위치 조정
		view.eye = glm::vec3(1.0f, 2.0f, 5.0f);
		view.at = glm::vec3(1.f, 2.f, 0.0f);
		view.up = glm::vec3(0.f, 1.f, 0.f);

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
		glClearColor(background_color.r, background_color.g, background_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int mode = GL_TRIANGLES;
		float angle = std::atan2f(view.at.z - view.eye.z, view.eye.x - view.at.x);
		background_image.clearMatrix(1);
		background_image.rotate(1, glm::degrees(angle) + 90.f, 0.f, 1.f, 0.f);
		background_image.translate(1, 0.f, playerstate.yPos, 0.f);
		background_image.draw(view, proj, mode, light);

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

	void Motion(const int _x, const int _y)
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
			if (playerstate.hold == true) {
				player.changeStatus(PLAYER_STAND);
				playerstate.hold = false;
			}
			else {
				// 플레이어 바로 앞에 블록이 있으면 홀드 자세를 취함
				if (playerstate.way == front) {
					if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos - 1)) {
						player.changeStatus(PLAYER_HOLD);
						playerstate.hold = true;
						playerstate.hold_brick_xPos = playerstate.xPos;
						playerstate.hold_brick_yPos = playerstate.yPos + 1;
						playerstate.hold_brick_zPos = playerstate.zPos - 1;
					}
				}
				else if (playerstate.way == back) {
					if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos + 1)) {
						player.changeStatus(PLAYER_HOLD);
						playerstate.hold = true;
						playerstate.hold_brick_xPos = playerstate.xPos;
						playerstate.hold_brick_yPos = playerstate.yPos + 1;
						playerstate.hold_brick_zPos = playerstate.zPos + 1;
					}
				}
				else if (playerstate.way == right) {
					if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos + 1, playerstate.zPos)) {
						player.changeStatus(PLAYER_HOLD);
						playerstate.hold = true;
						playerstate.hold_brick_xPos = playerstate.xPos + 1;
						playerstate.hold_brick_yPos = playerstate.yPos + 1;
						playerstate.hold_brick_zPos = playerstate.zPos;
					}
				}
				else if (playerstate.way == left) {
					if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos - 1, playerstate.zPos)) {
						player.changeStatus(PLAYER_HOLD);
						playerstate.hold = true;
						playerstate.hold_brick_xPos = playerstate.xPos - 1;
						playerstate.hold_brick_yPos = playerstate.yPos + 1;
						playerstate.hold_brick_zPos = playerstate.zPos;
					}
				}
			}
			PrintPos();
			break;
		case 'm':
			player.changeStatus(PLAYER_HANG);
			break;
		case 'v':
			// 블럭 느리게 밀기 테스트
			if (playerstate.hold) {
				playerstate.hard = true;
			}
			break;
		case ']':
			player.changeStatus(PLAYER_MOVING);
			break;
		}
	}

	void SpecialKeys(const int _key, const int _x, const int _y)
	{
		switch (_key) {
		case GLUT_KEY_UP:
			if (playerstate.hold) {
				if (moving_time == 0) {
					if (HoldCheckFront() == true) {
						moving_front_hold = true;
						playerstate.zPos -= 1;
						movewait_brick_yPos = 0;
						movewait_brick_xPos = 0;
						movewait_brick_zPos = -1;
					}
				}
				// 블럭 당기면 적절한 위치에 착지
				for (int i = 0; i < 5; i++) {
					if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos)) {
						bottom_adjustment = i;
						std::cout << bottom_adjustment << std::endl;
						return;
					}
				}
			}
			else if (CheckFront() == 1) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_MOVING);
					moving_front = true;
					playerstate.zPos -= 1;
				}
			}
			else if (CheckFront() == 2) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_front_up = true;
					playerstate.zPos -= 1;
					playerstate.yPos += 1;
				}
			}
			else if (CheckFront() == 3) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_front_down = true;
					playerstate.zPos -= 1;
					playerstate.yPos -= 1;
				}
			}
			if (playerstate.hold == false) {
				// 이전에 바라보던 방향에 맞게 현재 바라보는 방향 수정
				if (playerstate.way == right) {
					player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == left) {
					player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == back) {
					player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
				}
				// 현재 바라보는 방향 업데이트
				playerstate.way = front;
			}
			break;
		case GLUT_KEY_DOWN:
			if (playerstate.hold) {
				if (moving_time == 0) {
					if (HoldCheckBack() == true) {
						moving_back_hold = true;
						playerstate.zPos += 1;
						movewait_brick_yPos = 0;
						movewait_brick_xPos = 0;
						movewait_brick_zPos = 1;
					}
				}
				// 블럭 당기면 적절한 위치에 착지
				for (int i = 0; i < 5; i++) {
					if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos)) {
						bottom_adjustment = i;
						std::cout << bottom_adjustment << std::endl;
						return;
					}
				}
			}
			else if (CheckBack() == 1) {
				player.changeStatus(PLAYER_MOVING);
				if (moving_time == 0) {
					moving_back = true;
					playerstate.zPos += 1;
				}
			}
			else if (CheckBack() == 2) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_back_up = true;
					playerstate.zPos += 1;
					playerstate.yPos += 1;
				}
			}
			else if (CheckBack() == 3) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_back_down = true;
					playerstate.zPos += 1;
					playerstate.yPos -= 1;
				}
			}
			if (playerstate.hold == false) {
				if (playerstate.way == right) {
					player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == left) {
					player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == front) {
					player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
				}
				playerstate.way = back;
			}
			break;
		case GLUT_KEY_LEFT:
			if (playerstate.hold) {
				if (moving_time == 0) {
					if (HoldCheckLeft() == true) {
						moving_left_hold = true;
						playerstate.xPos -= 1;
						movewait_brick_yPos = 0;
						movewait_brick_xPos = -1;
						movewait_brick_zPos = 0;
					}
				}
				// 블럭 당기면 적절한 위치에 착지
				for (int i = 0; i < 5; i++) {
					if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos)) {
						bottom_adjustment = i;
						std::cout << bottom_adjustment << std::endl;
						return;
					}
				}
			}
			else if (CheckLeft() == 1) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_MOVING);
					moving_left = true;
					playerstate.xPos -= 1;
				}
			}
			else if (CheckLeft() == 2) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_left_up = true;
					playerstate.xPos -= 1;
					playerstate.yPos += 1;
				}
			}
			else if (CheckLeft() == 3) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_left_down = true;
					playerstate.xPos -= 1;
					playerstate.yPos -= 1;
				}
			}
			if (playerstate.hold == false) {
				if (playerstate.way == front) {
					player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == back) {
					player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == right) {
					player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
				}
				playerstate.way = left;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (playerstate.hold) {
				if (moving_time == 0) {
					if (HoldCheckRight() == true) {
						moving_right_hold = true;
						playerstate.xPos += 1;
						movewait_brick_yPos = 0;
						movewait_brick_xPos = 1;
						movewait_brick_zPos = 0;
					}
				}
				// 블럭 당기면 적절한 위치에 착지
				for (int i = 0; i < 5; i++) {
					if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos)) {
						bottom_adjustment = i;
						std::cout << bottom_adjustment << std::endl;
						return;
					}
				}
			}
			else if (CheckRight() == 1) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_MOVING);
					moving_right = true;
					playerstate.xPos += 1;
				}
			}
			else if (CheckRight() == 2) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_right_up = true;
					playerstate.xPos += 1;
					playerstate.yPos += 1;
				}
			}
			else if (CheckRight() == 3) {
				if (moving_time == 0) {
					player.changeStatus(PLAYER_HANG);
					moving_right_down = true;
					playerstate.xPos += 1;
					playerstate.yPos -= 1;
				}
			}
			if (playerstate.hold == false) {
				if (playerstate.way == front) {
					player.getShape().rotate(1, -90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == back) {
					player.getShape().rotate(1, 90.f, 0.f, 1.f, 0.f);
				}
				else if (playerstate.way == left) {
					player.getShape().rotate(1, 180.f, 0.f, 1.f, 0.f);
				}
				playerstate.way = right;
			}
			break;
		}
	}

	// 이동 검사
	int CheckFront()
	{
		// 이동 불가
		if (maps[current_map].isPosition(playerstate.yPos + 2, playerstate.xPos, playerstate.zPos - 1)) {
			return 0;
		}
		// 점프
		else if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos - 1)) {
			if (playerstate.way == front) {
				return 2;
			}
		}
		// 이동
		else if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos, playerstate.zPos - 1)) {
			return 1;
		}
		// 하단 점프
		else {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos - 1)) {
					bottom_adjustment = i - 1;
					return 3;
				}
			}
		}
		return 0;
	}
	int CheckBack()
	{
		if (maps[current_map].isPosition(playerstate.yPos + 2, playerstate.xPos, playerstate.zPos + 1)) {
			return 0;
		}
		else if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos + 1)) {
			if (playerstate.way == back) {
				return 2;
			}
		}
		else if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos, playerstate.zPos + 1)) {
			return 1;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos + 1)) {
					bottom_adjustment = i - 1;
					return 3;
				}
			}
		}
		return 0;
	}
	int CheckLeft()
	{
		if (maps[current_map].isPosition(playerstate.yPos + 2, playerstate.xPos - 1, playerstate.zPos)) {
			return 0;
		}
		else if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos - 1, playerstate.zPos)) {
			if (playerstate.way == left) {
				return 2;
			}
		}
		else if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos - 1, playerstate.zPos)) {
			return 1;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos - 1, playerstate.zPos)) {
					bottom_adjustment = i - 1;
					return 3;
				}
			}
		}
		return 0;
	}
	int CheckRight()
	{
		if (maps[current_map].isPosition(playerstate.yPos + 2, playerstate.xPos + 1, playerstate.zPos)) {
			return 0;
		}
		else if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos + 1, playerstate.zPos)) {
			if (playerstate.way == right) {
				return 2;
			}
		}
		else if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos + 1, playerstate.zPos)) {
			return 1;
		}
		else {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos + 1, playerstate.zPos)) {
					bottom_adjustment = i - 1;
					return 3;
				}
			}
		}
		return 0;
	}

	// 홀드한 채로 잘못된 이동 방지
	bool HoldCheckFront()
	{
		// 블럭 좌우 이동 방지
		if (playerstate.way == left || playerstate.way == right) {
			return false;
		}
		if (playerstate.way == front) {
			// 밀려는 곳에 이미 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos - 2)) {
				return false;
			}
			// 밀려는 곳에 발판이 없으면
			if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos, playerstate.zPos - 1) != true) {
				return false;
			}
		}
		if (playerstate.way == back) {
			// 당겨서 플레이어가 위치하는 곳에 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos - 1)) {
				return false;
			}
			// 착지가 불가능한가
			if (CanLanding() == false) {
				return false;
			}
		}
		return true;
	}
	bool HoldCheckBack()
	{
		// 블럭 좌우 이동 방지
		if (playerstate.way == left || playerstate.way == right) {
			return false;
		}
		if (playerstate.way == back) {
			// 밀려는 곳에 이미 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos + 2)) {
				return false;
			}
			// 밀려는 곳에 발판이 없으면
			if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos, playerstate.zPos + 1) != true) {
				return false;
			}
		}
		if (playerstate.way == front) {
			// 당겨서 플레이어가 위치하는 곳에 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos, playerstate.zPos + 1)) {
				return false;
			}
			// 착지가 불가능한가
			if (CanLanding() == false) {
				return false;
			}
		}
		return true;
	}
	bool HoldCheckLeft()
	{
		// 블럭 좌우 이동 방지
		if (playerstate.way == front || playerstate.way == back) {
			return false;
		}
		if (playerstate.way == left) {
			// 밀려는 곳에 이미 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos - 2, playerstate.zPos)) {
				return false;
			}
			// 밀려는 곳에 발판이 없으면
			if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos - 1, playerstate.zPos) != true) {
				return false;
			}
		}
		if (playerstate.way == right) {
			// 당겨서 플레이어가 위치하는 곳에 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos - 1, playerstate.zPos)) {
				return false;
			}
			// 착지가 불가능한가
			if (CanLanding() == false) {
				return false;
			}
		}
		return true;
	}
	bool HoldCheckRight()
	{
		// 블럭 좌우 이동 방지
		if (playerstate.way == front || playerstate.way == back) {
			return false;
		}
		if (playerstate.way == right) {
			// 밀려는 곳에 이미 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos + 2, playerstate.zPos)) {
				return false;
			}
			// 밀려는 곳에 발판이 없으면
			if (maps[current_map].isPosition(playerstate.yPos, playerstate.xPos + 1, playerstate.zPos) != true) {
				return false;
			}
		}
		if (playerstate.way == left) {
			// 당겨서 플레이어가 위치하는 곳에 블럭이 있으면
			if (maps[current_map].isPosition(playerstate.yPos + 1, playerstate.xPos + 1, playerstate.zPos)) {
				return false;
			}
			// 착지가 불가능한가
			if (CanLanding() == false) {
				return false;
			}
		}
		return true;
	}

	// 앞
	void MovingFront()
	{
		if (moving_front) {
			player.getShape().translate(3, 0.f, 0.f, -0.1f);
			view.eye.z -= 0.1f;
			view.at.z -= 0.1f;
			EndMove();
		}
	}
	void MovingFrontUp()
	{
		if (moving_front_up) {
			player.getShape().translate(3, 0.f, 0.1f, -0.1f);
			view.eye.z -= 0.1f;
			view.at.z -= 0.1f;
			view.eye.y += 0.1f;
			view.at.y += 0.1f;
			EndMove();
		}
	}
	void MovingFrontDown()
	{
		if (moving_front_down) {
			player.getShape().translate(3, 0.f, -0.1f, -0.1f);
			view.eye.z -= 0.1f;
			view.at.z -= 0.1f;
			view.eye.y -= 0.1f;
			view.at.y -= 0.1f;
			EndMove();
		}
	}
	void MovingFrontHold()
	{
		if (moving_front_hold) {
			if (playerstate.hard) {
				player.getShape().translate(3, 0.f, 0.f, -0.01f);
				view.eye.z -= 0.01f;
				view.at.z -= 0.01f;
			}
			else {
				player.getShape().translate(3, 0.f, 0.f, -0.1f);
				view.eye.z -= 0.1f;
				view.at.z -= 0.1f;
			}
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos - 1)) {
					bottom_adjustment = i - 1;
				}
			}
			EndMove();
		}
	}

	// 뒤
	void MovingBack()
	{
		if (moving_back) {
			player.getShape().translate(3, 0.f, 0.f, 0.1f);
			view.eye.z += 0.1f;
			view.at.z += 0.1f;
			EndMove();
		}
	}
	void MovingBackUp()
	{
		if (moving_back_up) {
			player.getShape().translate(3, 0.f, 0.1f, 0.1f);
			view.eye.z += 0.1f;
			view.at.z += 0.1f;
			view.eye.y += 0.1f;
			view.at.y += 0.1f;
			EndMove();
		}
	}
	void MovingBackDown()
	{
		if (moving_back_down) {
			player.getShape().translate(3, 0.f, -0.1f, 0.1f);
			view.eye.z += 0.1f;
			view.at.z += 0.1f;
			view.eye.y -= 0.1f;
			view.at.y -= 0.1f;
			EndMove();
		}
	}
	void MovingBackHold()
	{
		if (moving_back_hold) {
			if (playerstate.hard) {
				player.getShape().translate(3, 0.f, 0.f, 0.01f);
				view.eye.z += 0.01f;
				view.at.z += 0.01f;
			}
			else {
				player.getShape().translate(3, 0.f, 0.f, 0.1f);
				view.eye.z += 0.1f;
				view.at.z += 0.1f;
			}
			EndMove();
		}
	}

	// 왼쪽
	void MovingLeft()
	{
		if (moving_left) {
			player.getShape().translate(3, -0.1f, 0.f, 0.f);
			view.eye.x -= 0.1f;
			view.at.x -= 0.1f;
			EndMove();
		}
	}
	void MovingLeftUp()
	{
		if (moving_left_up) {
			player.getShape().translate(3, -0.1f, 0.1f, 0.f);
			view.eye.x -= 0.1f;
			view.at.x -= 0.1f;
			view.eye.y += 0.1;
			view.at.y += 0.1;
			EndMove();
		}
	}
	void MovingLeftDown()
	{
		if (moving_left_down) {
			player.getShape().translate(3, -0.1f, -0.1f, 0.f);
			view.eye.x -= 0.1f;
			view.at.x -= 0.1f;
			view.eye.y -= 0.1;
			view.at.y -= 0.1;
			EndMove();
		}
	}
	void MovingLeftHold()
	{
		if (moving_left_hold) {
			if (playerstate.hard) {
				player.getShape().translate(3, -0.01f, 0.f, 0.f);
				view.eye.x -= 0.01f;
				view.at.x -= 0.01f;
			}
			else {
				player.getShape().translate(3, -0.1f, 0.f, 0.f);
				view.eye.x -= 0.1f;
				view.at.x -= 0.1f;
			}
			EndMove();
		}
	}

	// 오른쪽
	void MovingRight()
	{
		if (moving_right) {
			player.getShape().translate(3, 0.1f, 0.f, 0.f);
			view.eye.x += 0.1f;
			view.at.x += 0.1f;
			EndMove();
		}
	}
	void MovingRightUp()
	{
		if (moving_right_up) {
			player.getShape().translate(3, 0.1f, 0.1f, 0.f);
			view.eye.x += 0.1f;
			view.at.x += 0.1f;
			view.eye.y += 0.1;
			view.at.y += 0.1;
			EndMove();
		}
	}
	void MovingRightDown()
	{
		if (moving_right_down) {
			player.getShape().translate(3, 0.1f, -0.1f, 0.f);
			view.eye.x += 0.1f;
			view.at.x += 0.1f;
			view.eye.y -= 0.1;
			view.at.y -= 0.1;
			EndMove();
		}
	}
	void MovingRightHold()
	{
		if (moving_right_hold) {
			if (playerstate.hard) {
				player.getShape().translate(3, 0.01f, 0.f, 0.f);
				view.eye.x += 0.01f;
				view.at.x += 0.01f;
			}
			else {
				player.getShape().translate(3, 0.1f, 0.f, 0.f);
				view.eye.x += 0.1f;
				view.at.x += 0.1f;
			}
			EndMove();
		}
	}

	// 무빙 종료 조건
	void EndMove()
	{
		if (playerstate.hard) {
			moving_time++;
		}
		else {
			moving_time += 10;
		}
		if (moving_time == 100) {
			player.changeStatus(PLAYER_STAND);
			moving_time = 0;
			moving_right = false;
			moving_left = false;
			moving_front = false;
			moving_back = false;

			moving_right_up = false;
			moving_left_up = false;
			moving_front_up = false;
			moving_back_up = false;

			moving_right_down = false;
			moving_left_down = false;
			moving_front_down = false;
			moving_back_down = false;

			moving_right_hold = false;
			moving_left_hold = false;
			moving_front_hold = false;
			moving_back_hold = false;

			playerstate.hard = false;

			if (playerstate.hold == true) {
				maps[current_map](playerstate.hold_brick_yPos, playerstate.hold_brick_xPos, playerstate.hold_brick_zPos).move(movewait_brick_yPos, movewait_brick_xPos, movewait_brick_zPos);
			}
			playerstate.hold = false;

			if (bottom_adjustment > 0) {
				BottomAdjustment(bottom_adjustment);
				bottom_adjustment = 0;
			}

			PrintPos();
		}
	}

	// 하단 조정
	void BottomAdjustment(int i)
	{
		playerstate.yPos -= i;
		player.getShape().translate(3, 0.f, -i, 0.f);
		view.eye.y -= i;
		view.at.y -= i;
	}

	// 착지가 가능한지 불가능한지 확인
	bool CanLanding()
	{
		if (playerstate.way == front) {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos + 1)) {
					return true;
				}
			}
		}
		else if (playerstate.way == back) {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos, playerstate.zPos - 1)) {
					return true;
				}
			}
		}
		else if (playerstate.way == left) {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos + 1, playerstate.zPos)) {
					return true;
				}
			}
		}
		else if (playerstate.way == right) {
			for (int i = 0; i < 5; i++) {
				if (maps[current_map].isPosition(playerstate.yPos - i, playerstate.xPos - 1, playerstate.zPos)) {
					return true;
				}
			}
		}
		return false;
	}

	// 플레이어 위치 확인용 코드
	void PrintPos() {
		std::cout << "-------------" << std::endl;
		std::cout << "yPos:" << playerstate.yPos << std::endl;
		std::cout << "xPos:" << playerstate.xPos << std::endl;
		std::cout << "zPos:" << playerstate.zPos << std::endl;
		if (playerstate.way == 0) {
			std::cout << "way: front" << std::endl;
		}
		else if (playerstate.way == 1) {
			std::cout << "way: right" << std::endl;
		}
		else if (playerstate.way == 2) {
			std::cout << "way: back" << std::endl;
		}
		else if (playerstate.way == 3) {
			std::cout << "way: left" << std::endl;
		}
		std::cout << "hold:" << playerstate.hold << std::endl;
	}

	// --
	// process func
	// --


	void updateState()
	{
		MovingFront();
		MovingBack();
		MovingLeft();
		MovingRight();

		MovingFrontUp();
		MovingBackUp();
		MovingLeftUp();
		MovingRightUp();

		MovingFrontDown();
		MovingBackDown();
		MovingLeftDown();
		MovingRightDown();

		MovingFrontHold();
		MovingBackHold();
		MovingLeftHold();
		MovingRightHold();
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