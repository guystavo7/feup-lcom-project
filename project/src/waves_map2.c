#include "../include/waves.h"

/**
 * Map 2: four paths (pathIndex 0..3) — lanes do not need to sync; each row is independent.
 * Duration 8 min. Last row at MAP2_DURATION_SEC: exactly one boss on one path; no other
 * spawns at that timestamp.
 * Table sorted by timeSec (then pathIndex for stable ordering at same second).
 */
const WaveSpawn WAVES_MAP2[] = {
    {0.0f, 0, MONSTER_KIND_PAWN, 2},
    {0.0f, 2, MONSTER_KIND_PAWN, 1},
    {20.0f, 1, MONSTER_KIND_ALIEN, 1},
    {35.0f, 3, MONSTER_KIND_PAWN, 2},
    {50.0f, 0, MONSTER_KIND_PAWN, 3},
    {50.0f, 2, MONSTER_KIND_ALIEN, 1},
    {80.0f, 1, MONSTER_KIND_LANCER, 1},
    {110.0f, 3, MONSTER_KIND_ALIEN, 2},
    {150.0f, 0, MONSTER_KIND_LANCER, 1},
    {180.0f, 2, MONSTER_KIND_PAWN, 4},
    {220.0f, 1, MONSTER_KIND_PAWN, 2},
    {220.0f, 3, MONSTER_KIND_LANCER, 1},
    {280.0f, 0, MONSTER_KIND_ALIEN, 2},
    {320.0f, 2, MONSTER_KIND_LANCER, 2},
    {380.0f, 1, MONSTER_KIND_PAWN, 5},
    {420.0f, 3, MONSTER_KIND_ALIEN, 3},
    {MAP2_DURATION_SEC, 0, MONSTER_KIND_BOSS, 1},
};

const unsigned int WAVES_MAP2_LEN = (unsigned int)(sizeof(WAVES_MAP2) / sizeof(WAVES_MAP2[0]));
