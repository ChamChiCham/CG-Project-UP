#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

// --
// Define variable
// --

enum
{

	// Display Mode
	WINDOW_DISPLAYMODE = GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,

	// Window Position
	WINDOW_POSITION_X = 50,
	WINDOW_POSITION_Y = 50,

	// Window Size
	WINDOW_SIZE_X = 1080,
	WINDOW_SIZE_Y = 720,

	// shape name
	SHAPE_DICE = 0,
	SHAPE_SQUARE_PRAMID,
	SHAPE_SQUARE,
	SHAPE_TRIANGLE,
	SHAPE_SPHERE, // �߽�: 0, 0.5, 0 / ����: 5.09116 .transform(0, 0.f, -0.05f, 0.f); .scale(1, 0.392837782.f,,);
	SHAPE_PLAYER_STAND,
	SHAPE_PLAYER_HOLD,
	SHAPE_PLAYER_HANG,
	SHAPE_MAX,

	PLAYER_STAND = 0,
	PLAYER_HOLD,
	PLAYER_HANG
};


constexpr const char* WINDOW_TITLE = "UP!!";