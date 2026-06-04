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
#include "../include/fruit.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"

#include "../include/ui/healthBar.h"
#include "../include/ui/statsBar.h"

#include "../resources/blue/tower/blueTower.h"
#include "../resources/fruits/fruits.h"
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

typedef struct {
    Collectable* collectable;
    FruitKind kind;
    float timeToLive;
} ArenaFruit;

static float dummy_proj_counter=1;

void projectileListFree(void* proj){
    Projectile* p=*(Projectile**)proj;
    projectileFree(p);
}

struct Arena {
    int* collidersN;
    BoxCollider*** colliders;

    int tilesN;
    Tile** tiles;

    int fruitsN;
    ArenaFruit* fruits;

    int towersN;
    GenericTower** towers;

    int width;
    int height;
    int tileSize;

    Monster* monster_test;
    MonsterGen* monsterGen1; 

    Vector* projectileList;
    Projectile* projectile_test;
 
    Path* path_test;

    Player* player;
    Cursor* cursor;

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

static char** arena_fruit_xpm_for_kind(FruitKind kind) {
    switch (kind) {
        case FRUIT_BANANA:
            return banana;
        case FRUIT_ABACATE:
            return abacate;
        case FRUIT_CEREJA:
            return cereja;
        case FRUIT_CENOURA:
            return cenoura;
        default:
            return banana;
    }
}

static void arena_fruits_sync_colliders(Arena* arena) {
    int n = arena->fruitsN;
    arena->collidersN[COLLECTABLE_TAG] = n;
    if (n == 0) {
        free(arena->colliders[COLLECTABLE_TAG]);
        arena->colliders[COLLECTABLE_TAG] = NULL;
        return;
    }
    BoxCollider** next = realloc(arena->colliders[COLLECTABLE_TAG], sizeof(BoxCollider*) * (size_t)n);
    if (next == NULL) return;
    arena->colliders[COLLECTABLE_TAG] = next;
    for (int i = 0; i < n; i++) {
        next[i] = collectableGetHitBox(arena->fruits[i].collectable);
    }
}

static void arena_fruit_push(Arena* arena, Vec2d position, FruitKind kind, float ttl_sec) {
    int idx = arena->fruitsN;
    arena->fruitsN++;
    ArenaFruit* grown = realloc(arena->fruits, sizeof(ArenaFruit) * (size_t)arena->fruitsN);
    if (grown == NULL) {
        arena->fruitsN--;
        return;
    }
    arena->fruits = grown;
    Collectable* c = collectableNew(position, EMPTY_VEC2D, FRUIT_SIZE, arena_fruit_xpm_for_kind(kind));
    arena->fruits[idx].collectable = c;
    arena->fruits[idx].kind = kind;
    arena->fruits[idx].timeToLive = ttl_sec;
    arena_fruits_sync_colliders(arena);
}

static void arena_fruit_remove_at(Arena* arena, int index) {
    if (index < 0 || index >= arena->fruitsN) return;
    collectableFree(arena->fruits[index].collectable);
    int last = arena->fruitsN - 1;
    if (index != last) arena->fruits[index] = arena->fruits[last];
    arena->fruitsN--;
    if (arena->fruitsN == 0) {
        free(arena->fruits);
        arena->fruits = NULL;
    } else {
        ArenaFruit* shrunk = realloc(arena->fruits, sizeof(ArenaFruit) * (size_t)arena->fruitsN);
        if (shrunk != NULL) arena->fruits = shrunk;
    }
    arena_fruits_sync_colliders(arena);
}

static void arena_fruits_tick_time_to_live(Arena* arena, float deltaTime) {
    for (int i = 0; i < arena->fruitsN;) {
        Collectable* c = arena->fruits[i].collectable;
        if (!collectableIsActive(c)) {
            arena_fruit_remove_at(arena, i);
            continue;
        }
        arena->fruits[i].timeToLive -= deltaTime;
        if (arena->fruits[i].timeToLive <= 0.0f) {
            arena_fruit_remove_at(arena, i);
            continue;
        }
        i++;
    }
}

static void arena_sync_fruit_stats_bar(Arena* arena) {
    const Player* p = arena->player;
    StatsBar* sb = arena->statsBar;
    if (p == NULL || sb == NULL) return;
    statsBarSetBanana(sb, (int)playerGetFruitCount(p, FRUIT_BANANA));
    statsBarSetAvocado(sb, (int)playerGetFruitCount(p, FRUIT_ABACATE));
    statsBarSetCherry(sb, (int)playerGetFruitCount(p, FRUIT_CEREJA));
    statsBarSetCarrot(sb, (int)playerGetFruitCount(p, FRUIT_CENOURA));
}

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
    int wallN = arena->collidersN[WALLS_TAG];
    int j;
    for (j = 0; j < wallN; j++) {
        if (arena->colliders[WALLS_TAG][j] == oldHit) break;
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
    arena->colliders[WALLS_TAG][j] = towerGetHitBox(neu);
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
    Vec2d fruitPos = {500, 400};
    arena_fruit_push(arena, fruitPos, FRUIT_ABACATE, FRUIT_TIME_ON_SCREEN_SEC);
}

Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile, const char* waterFile) {
    Arena* res = malloc(sizeof(Arena));
    res->collidersN = malloc(sizeof(int) * TAGS_NUM);
    res->colliders = malloc(sizeof(BoxCollider*) * TAGS_NUM);
    for (int t = 0; t < TAGS_NUM; t++) {
        res->collidersN[t] = 0;
        res->colliders[t] = NULL;
    }

    res->path_test = pathNew(4, vectorCreateWithFloatingData(sizeof(float), NULL, 7 * 2, 10.0f, 150.0f, 330.0f, 150.0f, 380.0f, 275.0f, 875.0f, 275.0f, 925.0f, 150.0f, 1300.0f, 150.0f, 1310.0f, 150.0f));
    res->monster_test = pawnMonsterNew(10, 150, res->path_test);
    res->monsterGen1 = monsterGenNew(res->path_test, defaultMonsterCreator, defaultGenTimeFunc);

    res->fruits = NULL;
    res->fruitsN = 0;

    SpriteXPM** projSpriteList = malloc(sizeof(SpriteXPM*));
    projSpriteList[0] = newSpriteXPM((xpm_map_t)Arrow2, XPM_8_8_8, 0, 0);
    AnimatedSprite* projAS = animatedSpriteNew(100, projSpriteList, 1, NORMAL);

    res->projectile_test = projectileCreate(PROJ_NORMAL, 200, 200, 200, -200, projAS);

    res->projectileList = vectorCreate(sizeof(Projectile*), projectileListFree);
  
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

	res->healthBar = healthBarNew(10);
	res->statsBar = statsBarNew();

    return res;
}

bool arenaCanPlaceTower(Arena* arena, Vec2d position) {
    BoxCollider* preview = boxColliderNew(position, BASIC_TOWER_HIT_BOX_OFFSET, BASIC_TOWER_SIZE, WALLS_TAG);
    BoxCollider* hits[PLACEMENT_QUERY_MAX];

    if (arenaDetectCollisions(arena, preview, WALLS_TAG, hits) > 0) {
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

    arena->collidersN[WALLS_TAG]++;
    arena->colliders[WALLS_TAG] = realloc(arena->colliders[WALLS_TAG], sizeof(BoxCollider*) * arena->collidersN[WALLS_TAG]);
    arena->colliders[WALLS_TAG][arena->collidersN[WALLS_TAG] - 1] = towerGetHitBox(arena->towers[arena->towersN - 1]);
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
    for (int i = 0; i < arena->fruitsN; i++) collectableFree(arena->fruits[i].collectable);
    free(arena->fruits);

    monsterFree(arena->monster_test);
    monsterGenFree(arena->monsterGen1);
    pathFree(arena->path_test);

    projectileFree(arena->projectile_test);

    free(arena->colliders[WALLS_TAG]);
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
    BoxCollider* collisions[PLACEMENT_QUERY_MAX];
    int n = arenaDetectCollisions(arena, hitBox, COLLECTABLE_TAG, collisions);
    for (int k = 0; k < n; k++) {
        int found = -1;
        for (int i = 0; i < arena->fruitsN; i++) {
            if (collectableGetHitBox(arena->fruits[i].collectable) == collisions[k]) {
                found = i;
                break;
            }
        }
        if (found < 0) break;
        playerAddFruit(arena->player, arena->fruits[found].kind);
        arena_fruit_remove_at(arena, found);
        arena_sync_fruit_stats_bar(arena);
    }
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
    BoxCollider* playerHitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    Vec2d position = {boxColliderGetX(playerHitBox), boxColliderGetY(playerHitBox)};
    Vec2d size = {boxColliderGetWidth(playerHitBox), boxColliderGetHeight(playerHitBox)};
    Vec2d center = {position.x + size.x / 2, position.y + size.y / 2};

    if (boxIsCollidingWithPoint(range, center)) return playerHitBox;
    return NULL;
}

void arenaHandleMonsterDamage(Arena* arena){

    int monsterIterator=monsterGenGetNextActiveMonsterIndex(arena->monsterGen1,0);

    while (monsterIterator!=-1)
    {
        Monster* monster=monsterGenGetMonster(arena->monsterGen1,monsterIterator);
        for(unsigned int i=0;i!=vectorGetSize(arena->projectileList);i++){
            Projectile* arrow= *(Projectile**)vectorGet(arena->projectileList,i);
            
            if(boxIsCollidingWithBox(projectileGetCollider(arrow),monsterGetBoxCollider(monster))){
                monsterSumHP(monster,-1000);
                projectileSetActive(arrow,false);
            }
        }
        monsterIterator++;
        monsterIterator=monsterGenGetNextActiveMonsterIndex(arena->monsterGen1,monsterIterator);
    }
    

}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    arena_fruits_tick_time_to_live(arena, deltaTime);
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i].collectable, terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);
    for (int i = arena->towersN - 1; i >= 0; i--) towerUpdate(arena->towers[i], terminal, arena, deltaTime); 
 
    //monster / proj tests

    arenaHandleMonsterDamage(arena);
    Vector* projIndexListToRemove=vectorCreate(sizeof(int),NULL);

    for(int i = vectorGetSize(arena->projectileList) - 1; i >= 0; i--){
        Projectile* p=*(Projectile**)vectorGet(arena->projectileList,i);
        projectileUpdate(p,deltaTime,terminal); 
        if(!projectileIsActive(p)){
            vectorPushBack(projIndexListToRemove,(char*)&i);
        }
    }

    for(int i =  0; i != (int)vectorGetSize(projIndexListToRemove); i++){
        int index=*(int*)vectorGet(projIndexListToRemove,i);
        vectorRemove(arena->projectileList,index);
    }

    vectorFree(projIndexListToRemove);

    dummy_proj_counter++;

    if(((int)dummy_proj_counter)%15==0){

        SpriteXPM** projSpriteList = malloc(sizeof(SpriteXPM*));
        projSpriteList[0] = newSpriteXPM((xpm_map_t)Arrow2, XPM_8_8_8, 0,0);
        AnimatedSprite* projAS=animatedSpriteNew(100,projSpriteList,1,NORMAL);
 
        Projectile* p=projectileCreate(PROJ_NORMAL,200,400,300,-300,projAS);
        vectorPushBack(arena->projectileList,(char*)&p);
    }

    

    monsterUpdate(arena->monster_test,arena,terminal,deltaTime); 
    projectileUpdate(arena->projectile_test,deltaTime,terminal);

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

	healthBarUpdate(arena->healthBar, terminal, arena);
	arena_sync_fruit_stats_bar(arena);
	statsBarUpdate(arena->statsBar, terminal, arena);

    cursorUpdate(getCursor(), terminal, deltaTime);
}
