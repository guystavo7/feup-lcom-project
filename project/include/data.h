#ifndef DATA
#define DATA

#include "utils/vec2d.h"

// Tags
#define WALLS_TAG 		0
#define PLAYER_TAG 		1
#define MONSTER_TAG		2
#define COLLECTABLE_TAG	3
#define PATH_TAG		4
#define UI_TAG			5
#define LAKE_TAG		6

#define TAGS_NUM 		7

// File Paths
static const char* map1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/map/grid_walls.csv";
static const char* path1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/map/grid_path.csv";
static const char* lake1 = "/home/lcom/labs/grupo_2leic13_2/project/resources/map/grid_water.csv";

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

// Basic tower
static const Vec2d BASIC_TOWER_RANGE = {300, 300};
static const Vec2d BASIC_TOWER_RANGE_OFFSET = {(60 - 300) / 2, (11 - 300) / 2};
#define BASIC_TOWER_COOLDOWN 0.5
#define BASIC_TOWER_SHOOT_SPEED 10 


// Fast Tower
static const Vec2d FAST_TOWER_RANGE = {350, 350};
static const Vec2d FAST_TOWER_RANGE_OFFSET = {(60 - 350) / 2, (11 - 350) / 2};
#define FAST_TOWER_COOLDOWN 0.1
#define FAST_TOWER_SHOOT_SPEED 20 


// Piercing Tower
static const Vec2d PIERCING_TOWER_RANGE = {350, 350};
static const Vec2d PIERCING_TOWER_RANGE_OFFSET = {(60 - 350) / 2, (11 - 350) / 2};
#define PIERCING_TOWER_COOLDOWN 0.7
#define PIERCING_TOWER_SHOOT_SPEED 10 


// Mortar Tower
static const Vec2d MORTAR_TOWER_RANGE = {600, 600};
static const Vec2d MORTAR_TOWER_RANGE_OFFSET = {(60 - 600) / 2, (11 - 600) / 2};
#define MORTAR_COOLDOWN 1.5 
#define MORTAR_TOWER_SHOOT_SPEED 10 


// Eletrict Tower
static const Vec2d ELETRICT_TOWER_RANGE = {150, 150};
static const Vec2d ELETRICT_TOWER_RANGE_OFFSET = {(60 - 150) / 2, (11 - 150) / 2};
#define ELETRICT_COOLDOWN 0.4
#define ELETRICT_TOWER_SHOOT_SPEED 15 


// Archer
#define ARCHER_FRAME_SPEED 10
#define ARCHER_IDLE_FRAMES 6
#define ARCHER_SHOOT_FRAMES 8
static const Vec2d ARCHER_OFFSET = {30 - 18, -27 - 44}; // x = tower center / 2 - archer / 2, y = upper tower some height - archer height

// Monk
#define MONK_FRAME_SPEED 6
#define MONK_IDLE_FRAMES 6
#define MONK_HEAL_FRAMES 11
static const Vec2d MONK_OFFSET = {30 - 14, -27 - 34};
static const Vec2d MONK_HEAL_OFFSET = {30 - 14 - 15, -27 - 34};

// Dust
#define DUST_FRAME_SPEED 10
#define DUST_FRAMES 9

// Cursor
#define CURSOR_FRAMES 4

// Tower upgrade menu (two scrolls side by side; XPM sizes from upgradeMenu.h)
#define UPGRADE_SCROLL_W 376
#define UPGRADE_SCROLL_H 307
#define UPGRADE_SCROLL_GAP 16
#define UPGRADE_SCROLL_TOTAL_W (UPGRADE_SCROLL_W + UPGRADE_SCROLL_GAP + UPGRADE_SCROLL_W)

#define UPGRADE_MENU_XPM_FAST 0
#define UPGRADE_MENU_XPM_PIERCING 1

#define UPGRADE_SCROLL_ORIGIN_X ((SCREEN_WIDTH - UPGRADE_SCROLL_TOTAL_W) / 2)
#define UPGRADE_SCROLL_ORIGIN_Y ((SCREEN_HEIGHT - UPGRADE_SCROLL_H) / 2)
#define UPGRADE_SCROLL_LEFT_X UPGRADE_SCROLL_ORIGIN_X
#define UPGRADE_SCROLL_RIGHT_X (UPGRADE_SCROLL_ORIGIN_X + UPGRADE_SCROLL_W + UPGRADE_SCROLL_GAP)

// Hammer button uses iconsXPM[] (Icon_01 / Icon_02)
#define UPGRADE_HAMMER_NORMAL 0
#define UPGRADE_HAMMER_PRESSED 1
#define UPGRADE_HAMMER_FRAMES 2
#define UPGRADE_HAMMER_FRAME_SPEED 10
#define UPGRADE_HAMMER_W 63
#define UPGRADE_HAMMER_H 60
#define UPGRADE_HAMMER_BOTTOM_MARGIN 50

#define UPGRADE_HAMMER_LEFT_X (UPGRADE_SCROLL_LEFT_X + (UPGRADE_SCROLL_W - UPGRADE_HAMMER_W) / 2)
#define UPGRADE_HAMMER_LEFT_Y (UPGRADE_SCROLL_ORIGIN_Y + UPGRADE_SCROLL_H - UPGRADE_HAMMER_H - UPGRADE_HAMMER_BOTTOM_MARGIN)
#define UPGRADE_HAMMER_RIGHT_X (UPGRADE_SCROLL_RIGHT_X + (UPGRADE_SCROLL_W - UPGRADE_HAMMER_W) / 2)
#define UPGRADE_HAMMER_RIGHT_Y UPGRADE_HAMMER_LEFT_Y

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

// Paused Menu
#define BUILD_NORMAL_BUTTON_INDEX 0
#define BUILD_PRESSED_BUTTON_INDEX 1
#define RESUME_NORMAL_BUTTON_INDEX 2
#define RESUME_PRESSED_BUTTON_INDEX 3
#define GO_BACK_NORMAl_BUTTON_INDEX 4
#define GO_BACK_PRESSED_BUTTON_INDEX 5
#define HELP_NORMAl_BUTTON_INDEX 6
#define HELP_PRESSED_BUTTON_INDEX 7

#define PAUSE_MENU_INDEX 0

static const Vec2d PAUSE_MENU_ICON_SIZE = {63, 60};
static const Vec2d PAUSE_MENU_BG_POS = {SCREEN_WIDTH / 2 - 295 / 2, SCREEN_HEIGHT / 2 - 200};
static const Vec2d RESUME_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 20, SCREEN_HEIGHT / 2 - 200 + 124};
static const Vec2d HELP_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 100, SCREEN_HEIGHT / 2 - 200 + 124};
static const Vec2d GO_BACK_POS = {SCREEN_WIDTH / 2 - 259 / 2 + 170, SCREEN_HEIGHT / 2 - 200 + 124};

#endif
