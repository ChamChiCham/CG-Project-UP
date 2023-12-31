#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

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
	SHAPE_PLAYER_MOVING,
	SHAPE_MAX,

	PLAYER_STAND = 0,
	PLAYER_HOLD,
	PLAYER_HANG,
	PLAYER_MOVING,

	BRICK_TYPE_NORMAL = 0,
	BRICK_TYPE_HARD,
	BRICK_TYPE_UNMOVABLE,
	BRICK_TYPE_ENDING,
	BRICK_TYPE_MAX,

	SHADER_COMMON_PROGRAM = 0,
	SHADER_TEXTURE_PROGRAM,

	MAP_MAX = 5,

	LAVA_SIZE = 7,

	SOUND_ITEM = 0,
	SOUND_JUMP,
	SOUND_PORTAL,
	SOUND_DEAD,
	SOUND_PUSH,
	SOUND_MAX,

	SOUND_ITEM_USE = 0
	
};


constexpr const char* WINDOW_TITLE = "UP!!";

constexpr const char* SHADER_COMMON_VERTEX = "glsl\\Vertex_common.glsl";
constexpr const char* SHADER_COMMON_FRAGMENT = "glsl\\Fragment_common.glsl";
constexpr const char* SHADER_TEXTURE_VERTEX = "glsl\\Vertex_texture.glsl";
constexpr const char* SHADER_TEXTURE_FRAGMENT = "glsl\\Fragment_texture.glsl";

constexpr const char* BRICK_IMAGE[4] = {
	"image\\normal_brick_con.png",
	"image\\hard_brick_con.png",
	"image\\motionless_brick_con.png",
	"image\\ending_brick_con.png"
};

constexpr const char* SOUND_NAME[SOUND_MAX] = {
	"sound\\item_get.mp3",
	"sound\\jump.mp3",
	"sound\\potal.mp3",
	"sound\\dead.mp3",
	"sound\\push.mp3",
};

constexpr const char* BACKGROUND_IMAGE = "image\\background.jpg";

constexpr const char* EFFECT_IMAGE = "image\\example.png";


extern const glm::vec3 PLAYER_COLOR_NORMAL;
extern const glm::vec3 PLAYER_COLOR_ITEM;


