#include "../include/arena.h"
#include "../include/buildings/basicLongTower.h"
#include "../include/buildings/basicTower.h"
#include "../include/buildings/eletricTower.h"
#include "../include/buildings/fastTower.h"
#include "../include/buildings/mortarTower.h"
#include "../include/buildings/piercingTower.h"
#include "../include/buildings/genericTower.h"

#include "../include/button.h"
#include "../include/components/boxCollider.h"
#include "../include/data.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"

#include "../include/ui/healthBar.h"
#include "../include/ui/statsBar.h"

#include "../resources/1.h"
#include "../resources/blue/tower/blueTower.h"
#include "../resources/icons/icons.h"
#include "../resources/map/background.h"
#include "../resources/tiles/tiles.h"
#include "../resources/upgradeMenu/upgradeMenu.h"

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

#include "../include/monster/alienMonster.h"
#include "../include/monster/monsterGen.h"
#include "../include/monster/pawnMonster.h"

#include "../resources/arrow/arrow.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ARENA_CONSTRUCT_NORMAL = 0,
    ARENA_CONSTRUCT_PICK_TOWER,
    ARENA_CONSTRUCT_PLACE_TOWER,
} ArenaConstructPhase;

typedef enum {
    ARENA_UPGRADE_IDLE = 0,
    ARENA_UPGRADE_MENU_OPEN,
} ArenaUpgradePhase;



struct Arena {
    int* collidersN;
    BoxCollider*** colliders;

    int tilesN;
    Tile** tiles;

    int fruitsN;
    Collectable** fruits;

    int towersN;
    GenericTower** towers;

    int width;
    int height;
    int tileSize;

    Monster* monster_test;
    MonsterGen* monsterGen1; 
 
    Path* path_test;

    Player* player;
    Cursor* cursor;

    int HeroHP;

	// UI
    HealthBar* healthBar;
	StatsBar* statsBar;

    ArenaConstructPhase constructPhase;
    int popupX;
    int popupY;
    int popupW;
    int popupH;
    int towerPickAnchorX;
    int towerPickAnchorY;
    SpriteXPM* towerPickUpperSprite;
    Button* towerPickButton;
    BoxCollider* towerPickHitbox;

    ArenaUpgradePhase upgradePhase;
    int upgradeTargetIndex;
    SpriteXPM* upgradeScrollLeft;
    SpriteXPM* upgradeScrollRight;
    BoxCollider* upgradeScrollLeftHitbox;
    BoxCollider* upgradeScrollRightHitbox;
    Button* upgradeHammerLeft;
    Button* upgradeHammerRight;
};

static void arenaTowerPickButtonCb(Button* self) {
    (void)self;
}

static Arena* s_arena_upgrade_button_ctx;

static void arenaCloseUpgradeMenu(Arena* arena) {
    arena->upgradePhase = ARENA_UPGRADE_IDLE;
    arena->upgradeTargetIndex = -1;
    buttonReset(arena->upgradeHammerLeft);
    buttonReset(arena->upgradeHammerRight);
}

static void arenaReplaceTargetTower(Arena* arena, TowerKind newKind) {
    int i = arena->upgradeTargetIndex;
    if (i < 0 || i >= arena->towersN) return;
    GenericTower* old = arena->towers[i];
    if (!towerIsBasic(old)) return;

    Vec2d pos = towerGetPosition(old);
    BoxCollider* oldHit = towerGetHitBox(old);
    int wallN = arena->collidersN[TOWER_TAG];
    int j;
    for (j = 0; j < wallN; j++) {
        if (arena->colliders[TOWER_TAG][j] == oldHit) break;
    }
    if (j >= wallN) return;

    GenericTower* neu = NULL;
    if (newKind == TOWER_KIND_FAST)
        neu = fastTowerNew(pos);
    else if (newKind == TOWER_KIND_PIERCING)
        neu = piercingTowerNew(pos);
    else
        return;

    arena->towers[i] = neu;
    arena->colliders[TOWER_TAG][j] = towerGetHitBox(neu);
    towerFree(old);
    arenaCloseUpgradeMenu(arena);
}

static void arenaUpgradeConfirmFastCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_FAST);
}

static void arenaUpgradeConfirmPiercingCb(Button* self) {
    (void)self;
    if (s_arena_upgrade_button_ctx) arenaReplaceTargetTower(s_arena_upgrade_button_ctx, TOWER_KIND_PIERCING);
}

static void arenaTryOpenUpgradeFromTowerClick(Arena* arena) {
    if (arena->constructPhase != ARENA_CONSTRUCT_NORMAL || arena->upgradePhase != ARENA_UPGRADE_IDLE) return;
    Cursor* cur = getCursor();
    if (!cursorIsLeftButtonPressedDown(cur)) return;

    int cx = cursorGetX(cur);
    int cy = cursorGetY(cur);
    Vec2d click = {(float)cx, (float)cy};

    for (int ti = arena->towersN - 1; ti >= 0; ti--) {
        GenericTower* t = arena->towers[ti];
        if (!towerIsBasic(t)) continue;
        if (!boxIsCollidingWithPoint(towerGetHitBox(t), click)) continue;
        arena->upgradePhase = ARENA_UPGRADE_MENU_OPEN;
        arena->upgradeTargetIndex = ti;
        buttonReset(arena->upgradeHammerLeft);
        buttonReset(arena->upgradeHammerRight);
        return;
    }
}

static void arenaHandleUpgradeUi(Arena* arena) {
    s_arena_upgrade_button_ctx = arena;
    Cursor* cur = getCursor();
    Vec2d mousePos = {(float)cursorGetX(cur), (float)cursorGetY(cur)};
    bool leftClick = cursorIsLeftButtonPressedDown(cur);
    int mx = (int)mousePos.x;
    int my = (int)mousePos.y;

    if (buttonContainsPoint(arena->upgradeHammerLeft, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerLeft);
        buttonHover(arena->upgradeHammerLeft);
    } else {
        buttonReset(arena->upgradeHammerLeft);
    }

    if (buttonContainsPoint(arena->upgradeHammerRight, mx, my)) {
        if (leftClick) buttonClick(arena->upgradeHammerRight);
        buttonHover(arena->upgradeHammerRight);
    } else {
        buttonReset(arena->upgradeHammerRight);
    }

    if (leftClick) {
        if (buttonContainsPoint(arena->upgradeHammerLeft, mx, my) || buttonContainsPoint(arena->upgradeHammerRight, mx, my)) {
            s_arena_upgrade_button_ctx = NULL;
            return;
        }
        if (!boxIsCollidingWithPoint(arena->upgradeScrollLeftHitbox, mousePos) && !boxIsCollidingWithPoint(arena->upgradeScrollRightHitbox, mousePos))
            arenaCloseUpgradeMenu(arena);
    }

    s_arena_upgrade_button_ctx = NULL;
}

static void arenaDrawUpgradeMenu(Arena* arena, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(arena->upgradeScrollLeft, terminal, UPGRADE_SCROLL_LEFT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    drawSpriteXPM(arena->upgradeScrollRight, terminal, UPGRADE_SCROLL_RIGHT_X, UPGRADE_SCROLL_ORIGIN_Y, false);
    buttonDraw(arena->upgradeHammerLeft, terminal, deltaTime, false);
    buttonDraw(arena->upgradeHammerRight, terminal, deltaTime, false);
}

static bool arenaPointInPopup(Arena* arena, int px, int py) {
    return px >= arena->popupX && px < arena->popupX + arena->popupW && py >= arena->popupY && py < arena->popupY + arena->popupH;
}

static void createMap(Arena* arena, const char* filename, int tileSize) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];

    arena->tiles = malloc(sizeof(Tile*) * (MAP_TILES_X * MAP_TILES_Y + 4));
    arena->width = MAP_TILES_X;
    arena->height = MAP_TILES_Y;
    arena->tileSize = tileSize;

    int** map = malloc(arena->height * sizeof(int*));
    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            char** sprite = tiles[id - 1];
            arena->tiles[i] = tileNew(sprite, id, x * tileSize, y * tileSize, tileSize, tileSize);
            i++;
        }
    }

    arena->tiles[i] = tileNew(NULL, -1, 0, -10, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 1] = tileNew(NULL, -1, 0, MAP_TILES_Y * TILE_SIZE, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 2] = tileNew(NULL, -1, -10, 0, 10, MAP_TILES_Y * TILE_SIZE);
    arena->tiles[i + 3] = tileNew(NULL, -1, MAP_TILES_X * TILE_SIZE, 0, 10, MAP_TILES_Y * TILE_SIZE);
    i += 4;
    arena->tilesN = i;

    arena->collidersN[WALLS_TAG] = arena->tilesN;
    arena->colliders[WALLS_TAG] = malloc(sizeof(BoxCollider*) * (i + 4));
    for (int i = 0; i < arena->height; i++) free(map[i]);
    for (int j = 0; j < arena->tilesN; j++) arena->colliders[WALLS_TAG][j] = tileBoxCollider(arena->tiles[j]);

    free(map);
}

static void createMonsterPathColliders(Arena* arena, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];
    int** map = malloc(arena->height * sizeof(int*));
    int num = 0;

    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            if (map[i][j] > 0) num++;

            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    arena->collidersN[PATH_TAG] = num;
    arena->colliders[PATH_TAG] = malloc(sizeof(BoxCollider*) * num);
    int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            Vec2d position = {x * TILE_SIZE, y * TILE_SIZE};
            Vec2d size = {TILE_SIZE, TILE_SIZE};
            arena->colliders[PATH_TAG][i] = boxColliderNew(position, EMPTY_VEC2D, size, PATH_TAG);
            i++;
        }
    }

    for (int i = 0; i < arena->height; i++) free(map[i]);
    free(map);
}

static void createLakeColliders(Arena* arena, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];
    int** map = malloc(arena->height * sizeof(int*));
    int num = 0;

    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            if (map[i][j] > 0) num++;

            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    arena->collidersN[LAKE_TAG] = num;
    arena->colliders[LAKE_TAG] = malloc(sizeof(BoxCollider*) * num);
    int idx = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            Vec2d position = {x * TILE_SIZE, y * TILE_SIZE};
            Vec2d size = {TILE_SIZE, TILE_SIZE};
            arena->colliders[LAKE_TAG][idx] = boxColliderNew(position, EMPTY_VEC2D, size, LAKE_TAG);
            idx++;
        }
    }

    for (int i = 0; i < arena->height; i++) free(map[i]);
    free(map);
}

static void createPlayer(Arena* arena) {
    Vec2d playerPos = {100, 100};
    arena->player = playerNew(playerPos);

    arena->colliders[PLAYER_TAG] = malloc(sizeof(BoxCollider*));
    arena->colliders[PLAYER_TAG][0] = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    arena->collidersN[PLAYER_TAG] = 1;
}

static void createFruitsTest(Arena* arena) {
    arena->fruitsN = 1;
    arena->fruits = malloc(sizeof(Collectable*) * arena->fruitsN);
    Vec2d fruitPos = {500, 400};

    arena->fruits[0] = collectableNew(fruitPos, EMPTY_VEC2D, FRUIT_SIZE, abc);
    arena->colliders[COLLECTABLE_TAG] = malloc(sizeof(BoxCollider*) * arena->fruitsN);
    arena->collidersN[COLLECTABLE_TAG] = arena->fruitsN;
    for (int i = 0; i < arena->fruitsN; i++) arena->colliders[COLLECTABLE_TAG][i] = collectableGetHitBox(arena->fruits[i]);
}

Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile, const char* waterFile) {
    Arena* res = malloc(sizeof(Arena));
    res->collidersN = malloc(sizeof(int) * TAGS_NUM);
    res->colliders = malloc(sizeof(BoxCollider*) * TAGS_NUM);
    for (int t = 0; t < TAGS_NUM; t++) {
        res->collidersN[t] = 0;
        res->colliders[t] = NULL;
    } 
 
    res->path_test=pathNew(4,vectorCreateWithFloatingData(sizeof(float),NULL,7*2, 10.0f,150.0f,  330.0f,150.0f,  380.0f,275.0f,  875.0f,275.0f,   925.0f,150.0f,  1300.0f,150.0f, 1310.0f,150.0f ));
    res->monster_test=pawnMonsterNew(10,150,res->path_test);
    res->monsterGen1=monsterGenNew(res->path_test,defaultMonsterCreator,defaultGenTimeFunc);

    res->collidersN[MONSTER_TAG] = 1;
    res->colliders[MONSTER_TAG] = malloc(sizeof(BoxCollider*) * res->collidersN[MONSTER_TAG]);
    res->colliders[MONSTER_TAG][0] = monsterGetBoxCollider(res->monster_test);

    createMap(res, tilesFile, TILE_SIZE);
    createMonsterPathColliders(res, pathFile);
    createLakeColliders(res, waterFile);
    createPlayer(res);
    createFruitsTest(res);
    res->towersN = 0;
    res->towers = malloc(sizeof(GenericTower*) * res->towersN);

    res->constructPhase = ARENA_CONSTRUCT_NORMAL;
    res->popupW = 420;
    res->popupH = 240;
    res->popupX = (SCREEN_WIDTH - res->popupW) / 2;
    res->popupY = (SCREEN_HEIGHT - res->popupH) / 2;

    /* Anchor = canto sup.-esq. da metade inferior (igual à torre no jogo); a metade superior sobe com offset Y. */
    int ix = res->popupX + res->popupW - TOWER_PICK_ICON_W - POPUP_ICON_MARGIN;
    int iy = res->popupY + POPUP_ICON_MARGIN - (int)BASIC_UPPER_HALF_OFFSET.y;
    res->towerPickAnchorX = ix;
    res->towerPickAnchorY = iy;

    res->towerPickUpperSprite = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** sprites = malloc(sizeof(SpriteXPM*));
    sprites[0] = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    res->towerPickButton = buttonNew(ix, iy, BASIC_TOWER_SIZE.x, BASIC_TOWER_SIZE.y, animatedSpriteNew(0, sprites, 1, NORMAL), arenaTowerPickButtonCb, arenaTowerPickButtonCb);

    float hitY = (float)(res->popupY + POPUP_ICON_MARGIN);
    float hitH = 33.f - BASIC_UPPER_HALF_OFFSET.y;
    res->towerPickHitbox = boxColliderNew((Vec2d){(float)ix, hitY}, EMPTY_VEC2D, (Vec2d){(float)TOWER_PICK_ICON_W, hitH}, 0);

    res->upgradePhase = ARENA_UPGRADE_IDLE;
    res->upgradeTargetIndex = -1;
    res->upgradeScrollLeft = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_FAST], XPM_8_8_8, 0, 0);
    res->upgradeScrollRight = newSpriteXPM((xpm_map_t)upgradeMenuXPM[UPGRADE_MENU_XPM_PIERCING], XPM_8_8_8, 0, 0);
    res->upgradeScrollLeftHitbox = boxColliderNew((Vec2d){(float)UPGRADE_SCROLL_LEFT_X, (float)UPGRADE_SCROLL_ORIGIN_Y}, EMPTY_VEC2D, (Vec2d){(float)UPGRADE_SCROLL_W, (float)UPGRADE_SCROLL_H}, UI_TAG);
    res->upgradeScrollRightHitbox = boxColliderNew((Vec2d){(float)UPGRADE_SCROLL_RIGHT_X, (float)UPGRADE_SCROLL_ORIGIN_Y}, EMPTY_VEC2D, (Vec2d){(float)UPGRADE_SCROLL_W, (float)UPGRADE_SCROLL_H}, UI_TAG);

    SpriteXPM** hammerSpritesLeft = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesLeft[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesLeft[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerLeft =
        buttonNew(UPGRADE_HAMMER_LEFT_X, UPGRADE_HAMMER_LEFT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H,
                  animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesLeft, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmFastCb, basicButtonHoverFn);

    SpriteXPM** hammerSpritesRight = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    hammerSpritesRight[0] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_NORMAL], XPM_8_8_8, 0, 0);
    hammerSpritesRight[1] = newSpriteXPM((xpm_map_t)iconsXPM[UPGRADE_HAMMER_PRESSED], XPM_8_8_8, 0, 0);
    res->upgradeHammerRight =
        buttonNew(UPGRADE_HAMMER_RIGHT_X, UPGRADE_HAMMER_RIGHT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H,
                  animatedSpriteNew((float)UPGRADE_HAMMER_FRAME_SPEED, hammerSpritesRight, UPGRADE_HAMMER_FRAMES, NORMAL), arenaUpgradeConfirmPiercingCb, basicButtonHoverFn);

    SpriteXPM* backgroundSprite = newSpriteXPM((xpm_map_t)background, XPM_8_8_8, 0, 0);
    generateEmptyBuffer(backgroundSprite, terminal, 0, 0, false);

    res->HeroHP=10;
	res->healthBar = healthBarNew(res->HeroHP);
	res->statsBar = statsBarNew();

    return res;
}

bool arenaCanPlaceTower(Arena* arena, Vec2d position) {
    BoxCollider* preview = boxColliderNew(position, BASIC_TOWER_HIT_BOX_OFFSET, BASIC_TOWER_SIZE, TOWER_TAG);
    BoxCollider* hits[PLACEMENT_QUERY_MAX];

    if (arenaDetectCollisions(arena, preview, WALLS_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, TOWER_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, PLAYER_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, PATH_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }
    if (arenaDetectCollisions(arena, preview, LAKE_TAG, hits) > 0) {
        boxColliderFree(preview);
        return false;
    }

    boxColliderFree(preview);
    return true;
}

void arenaBuildTower(Arena* arena, Vec2d position) {
    if (!arenaCanPlaceTower(arena, position)) return;

    arena->towersN++;
    arena->towers = realloc(arena->towers, sizeof(GenericTower*) * arena->towersN);
    arena->towers[arena->towersN - 1] = basicLongTowerNew(position);

    arena->collidersN[TOWER_TAG]++;
    arena->colliders[TOWER_TAG] = realloc(arena->colliders[TOWER_TAG], sizeof(BoxCollider*) * arena->collidersN[TOWER_TAG]);
    arena->colliders[TOWER_TAG][arena->collidersN[TOWER_TAG] - 1] = towerGetHitBox(arena->towers[arena->towersN - 1]);
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->collidersN[PATH_TAG]; i++) boxColliderFree(arena->colliders[PATH_TAG][i]);
    free(arena->colliders[PATH_TAG]);

    for (int i = 0; i < arena->collidersN[LAKE_TAG]; i++) boxColliderFree(arena->colliders[LAKE_TAG][i]);
    free(arena->colliders[LAKE_TAG]);

    for (int i = 0; i < arena->towersN; i++) towerFree(arena->towers[i]);
    free(arena->towers);

    freeSpriteXPM(arena->upgradeScrollLeft);
    freeSpriteXPM(arena->upgradeScrollRight);
    boxColliderFree(arena->upgradeScrollLeftHitbox);
    boxColliderFree(arena->upgradeScrollRightHitbox);
    buttonFree(arena->upgradeHammerLeft);
    buttonFree(arena->upgradeHammerRight);

    freeSpriteXPM(arena->towerPickUpperSprite);
    boxColliderFree(arena->towerPickHitbox);
    buttonFree(arena->towerPickButton);

    for (int i = 0; i < arena->tilesN; i++) tileFree(arena->tiles[i]);
    for (int i = 0; i < arena->fruitsN; i++) collectableFree(arena->fruits[i]);

    monsterFree(arena->monster_test);
    monsterGenFree(arena->monsterGen1);
    pathFree(arena->path_test);

    free(arena->colliders[WALLS_TAG]);
    free(arena->colliders[TOWER_TAG]);
    free(arena->colliders[PLAYER_TAG]);
    free(arena->colliders[COLLECTABLE_TAG]);
    free(arena->tiles);

    free(arena->fruits);
    playerFree(arena->player);

    free(arena->colliders);
    free(arena->collidersN);
	healthBarFree(arena->healthBar);
	statsBarFree(arena->statsBar);
	
    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    if (tag < 0 || tag >= TAGS_NUM) return 0;
    int n = arena->collidersN[tag];
    BoxCollider** c = arena->colliders[tag];
    if (c == NULL || n == 0) return 0;

    int j = 0;
    for (int i = 0; i < n; i++) {
        BoxCollider* collider = c[i];
        if (collider == NULL) continue;
        if (boxIsCollidingWithBox(box, collider)) {
            colliders[j] = collider;
            j++;
            if (j >= PLACEMENT_QUERY_MAX) break;
        }
    }

    return j;
}

static void arenaCollectableHandler(Arena* arena) {
    BoxCollider* hitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    BoxCollider* collisions[arena->fruitsN];
    int n = arenaDetectCollisions(arena, hitBox, COLLECTABLE_TAG, collisions);
    for (int i = 0; i < n; i++) collectableSetActive(arena->fruits[i], false);
}

static void arenaDrawConstructionPopup(Arena* arena, Terminal* terminal, float deltaTime) {
    drawRectangle(terminal, arena->popupX, arena->popupY, (uint16_t)arena->popupW, (uint16_t)arena->popupH, 0x202028);
    buttonDraw(arena->towerPickButton, terminal, deltaTime, false);
    drawSpriteXPM(arena->towerPickUpperSprite, terminal, arena->towerPickAnchorX, arena->towerPickAnchorY, false);
}

static void arenaHandleConstruction(Arena* arena, Terminal* terminal) {
    Cursor* cur = getCursor();
    (void)terminal;

    if (arena->upgradePhase == ARENA_UPGRADE_MENU_OPEN) {
        arenaHandleUpgradeUi(arena);
        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_NORMAL) {
        cursorSetConstructionMode(cur, false);
        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_PICK_TOWER;
            return;
        }
        arenaTryOpenUpgradeFromTowerClick(arena);
        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_PICK_TOWER) {
        cursorSetConstructionMode(cur, false);
        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
            return;
        }
        if (cursorIsLeftButtonPressedDown(cur)) {
            int cx = cursorGetX(cur);
            int cy = cursorGetY(cur);
            Vec2d click = {(float)cx, (float)cy};
            if (boxIsCollidingWithPoint(arena->towerPickHitbox, click)) {
                arena->constructPhase = ARENA_CONSTRUCT_PLACE_TOWER;
                return;
            }
            if (!arenaPointInPopup(arena, cx, cy)) {
                arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
            }
        }
        return;
    }

    if (arena->constructPhase == ARENA_CONSTRUCT_PLACE_TOWER) {
        cursorSetConstructionMode(cur, true);
        Vec2d pos = {(float)cursorGetX(cur), (float)cursorGetY(cur)};
        bool ok = arenaCanPlaceTower(arena, pos);
        cursorSetPlacementHoverValid(cur, ok);

        if (cursorIsRightButtonPressedDown(cur)) {
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
            cursorSetConstructionMode(cur, false);
            return;
        }
        if (cursorIsLeftButtonPressedDown(cur) && ok) {
            arenaBuildTower(arena, pos);
            cursorSetConstructionMode(cur, false);
            arena->constructPhase = ARENA_CONSTRUCT_NORMAL;
        }
    }
}

BoxCollider* arenaFindFirstEnemy(Arena* arena, BoxCollider* range) {
    int monsterIterator = monsterGenGetNextActiveMonsterIndex(arena->monsterGen1, 0);
    while (monsterIterator != -1) {
        Monster* monster = monsterGenGetMonster(arena->monsterGen1, monsterIterator);
        BoxCollider* monsterCollider = monsterGetBoxCollider(monster);
        Vec2d position = {boxColliderGetX(monsterCollider), boxColliderGetY(monsterCollider)};
        Vec2d size = {boxColliderGetWidth(monsterCollider), boxColliderGetHeight(monsterCollider)};
        Vec2d center = {position.x + size.x / 2, position.y + size.y / 2};

        if (boxIsCollidingWithPoint(range, center)) {
            return monsterCollider;
        }
        monsterIterator++;
        monsterIterator = monsterGenGetNextActiveMonsterIndex(arena->monsterGen1, monsterIterator);
    }

    return NULL;
}

void arenaHandleMonsterDamage(Arena* arena){

    int monsterIterator=monsterGenGetNextActiveMonsterIndex(arena->monsterGen1,0);

    while (monsterIterator!=-1)
    {
        Monster* monster=monsterGenGetMonster(arena->monsterGen1,monsterIterator);
        

        for(int t=0; t<arena->towersN; t++){
            GenericTower* tower = arena->towers[t];
            int projCount = towerGetProjectilesCount(tower);
            for(int i=0; i<projCount; i++){
                Projectile* arrow = towerGetProjectile(tower, i);
                if (!projectileIsActive(arrow)) continue;
                
                if(boxIsCollidingWithBox(projectileGetCollider(arrow),monsterGetBoxCollider(monster))){
                    monsterSumHP(monster,-1000);
                    projectileAddPierce(arrow);
                }
            }
        }

        monsterIterator++;
        monsterIterator=monsterGenGetNextActiveMonsterIndex(arena->monsterGen1,monsterIterator);
    }
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i], terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);
    for (int i = arena->towersN - 1; i >= 0; i--) towerUpdate(arena->towers[i], terminal, arena, deltaTime); 
 
    //monster / proj tests

    arenaHandleMonsterDamage(arena);
    

    monsterUpdate(arena->monster_test,arena,terminal,deltaTime); 

    monsterGenUpdate(arena->monsterGen1,arena,terminal,deltaTime);

    //monster / proj tests
 
    arenaCollectableHandler(arena);
    arenaHandleConstruction(arena, terminal);

    playerUpdate(arena->player, terminal, arena, deltaTime);
    for (int i = arena->towersN - 1; i >= 0; i--) towerDrawUpperHalf(arena->towers[i], terminal, arena, deltaTime);

    if (arena->upgradePhase == ARENA_UPGRADE_MENU_OPEN) {
        arenaDrawUpgradeMenu(arena, terminal, deltaTime);
    } else if (arena->constructPhase == ARENA_CONSTRUCT_PICK_TOWER) {
        arenaDrawConstructionPopup(arena, terminal, deltaTime);
    }

    
    arena->HeroHP-=monsterGenGetNumberOfInstantFinishedMonsters(arena->monsterGen1);
    if(!(arena->HeroHP>0)){
        arena->HeroHP=0;
    }

    healthBarSetHealth(arena->healthBar,arena->HeroHP);
	healthBarUpdate(arena->healthBar, terminal, arena);
	statsBarUpdate(arena->statsBar, terminal, arena);

    cursorUpdate(getCursor(), terminal, deltaTime);
}
