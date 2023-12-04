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
	SHAPE_SPHERE, // 중심: 0, 0.5, 0 / 지름: 5.09116 .transform(0, 0.f, -0.05f, 0.f); .scale(1, 0.392837782.f,,);
	SHAPE_PLAYER_STAND,
	SHAPE_PLAYER_HOLD,
	SHAPE_PLAYER_HANG,
	SHAPE_MAX,

	PLAYER_STAND = 0,
	PLAYER_HOLD,
	PLAYER_HANG,

	BRICK_TYPE_NORMAL = 0,
	BRICK_TYPE_HARD,
	BRICK_TYPE_UNMOVABLE,
	BRICK_TYPE_MAX,

	SHADER_COMMON_PROGRAM = 0,
	SHADER_TEXTURE_PROGRAM
};


constexpr const char* WINDOW_TITLE = "UP!!";

constexpr const char* SHADER_COMMON_VERTEX = "glsl\\Vertex_common.glsl";
constexpr const char* SHADER_COMMON_FRAGMENT = "glsl\\Fragment_common.glsl";
constexpr const char* SHADER_TEXTURE_VERTEX = "glsl\\Vertex_texture.glsl";
constexpr const char* SHADER_TEXTURE_FRAGMENT = "glsl\\Fragment_texture.glsl";

constexpr const char* BRICK_IMAGE[3] = {
	"image\\normal_brick.png",
	"image\\hard_brick.png",
	"image\\motionless_brick.png"
};

constexpr const char* BACKGROUND_IMAGE = "image\\background.jpg";



