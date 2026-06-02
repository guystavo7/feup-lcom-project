#ifndef DATA
#define DATA

#include "utils/vec2d.h"

// File Paths
static const char* map1 = "/home/lcom/labs/project/resources/map/grid_walls.csv";
static const char* path1 = "/home/lcom/labs/project/resources/map/grid_path.csv";

// Tags
#define WALLS_TAG 		0
#define PLAYER_TAG 		1
#define MONSTER_TAG		2
#define COLLECTABLE_TAG	3
#define PATH_TAG		4
#define UI_TAG			5

#define TAGS_NUM 		6

// Screen configs
#define MODE 0x11B
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define MAP_TILES_X 40
#define MAP_TILES_Y 32
#define TILE_SIZE 32

static const Vec2d EMPTY_VEC2D = {0, 0};

// Fruit
static const Vec2d FRUIT_SIZE = {32, 32};

// Player
#define PLAYER_MAX_VELOCITY 200
#define PLAYER_FRAME_SPEED 10
#define PLAYER_IDLE_FRAMES 8
#define PLAYER_RUNNING_FRAMES 6
static const Vec2d PLAYER_HIT_BOX_OFFSET = {10, 33};
static const Vec2d PLAYER_SIZE = {17, 13};

// Tower
static const Vec2d BASIC_TOWER_SIZE = {43, 29};
static const Vec2d BASIC_TOWER_HIT_BOX_OFFSET = {9, 0};
static const Vec2d BASIC_UPPER_HALF_OFFSET = {0, -59};

static const Vec2d BASIC_TOWER_RANGE = {300, 300};
static const Vec2d BASIC_TOWER_RANGE_OFFSET = {(60 - 300) / 2, (11 - 300) / 2};
#define BASIC_TOWER_COOLDOWN 0.5

// Archer
#define ARCHER_FRAME_SPEED 10
#define ARCHER_IDLE_FRAMES 6
#define ARCHER_SHOOT_FRAMES 8
static const Vec2d ARCHER_OFFSET = {30 - 18, -27 - 44}; // x = tower center / 2 - archer / 2, y = upper tower some height - archer height
								
// Monk
#define MONK_FRAME_SPEED 6
#define MONK_IDLE_FRAMES 6
#define MONK_HEAL_FRAMES 12
static const Vec2d MONK_OFFSET = {30 - 14, -27 - 34}; 

// Dust
#define DUST_FRAME_SPEED 10
#define DUST_FRAMES 9

// Cursor
#define CURSOR_FRAMES 3

// Main Menu
#define NEW_GAME_BUTTON_NORMAL 0
#define NEW_GAME_BUTTON_PRESSED 1
#define EXIT_BUTTON_NORMAL 2
#define EXIT_BUTTON_PRESSED 3
#define LOGO 4

#define NEW_GAME_BUTTON_FRAMES 2
#define EXIT_BUTTON_FRAMES 2
#define BUTTONS_FRAME_SPEED 10

static const Vec2d LOGO_POSITION = {445, 200};

static const Vec2d NEW_GAME_BUTTON_POS = {499, 400};
static const Vec2d NEW_GAME_BUTTON_SIZE = {282, 94};
static const Vec2d EXIT_BUTTON_POS = {499, 550};
static const Vec2d EXIT_BUTTON_SIZE = {282, 94};

#endif
