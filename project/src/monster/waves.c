#include "../../include/monster/waves.h"

#include <stddef.h>

#define NORMAL_WAVES 7
#define MAP1_DURATION_SEC (7u * 60u)
#define MAP23_DURATION_SEC (8u * 60u)

static const WaveComposition MAP1_WAVES[NORMAL_WAVES] = {
    {12u, 0u, 0u},
    {15u, 9u, 6u},
    {15u, 12u, 6u},
    {18u, 12u, 9u},
    {18u, 15u, 9u},
    {21u, 15u, 9u},
    {24u, 18u, 9u},
};

static const WaveComposition MAP2_WAVES[NORMAL_WAVES] = {
    {12u, 0u, 0u},
    {15u, 12u, 6u},
    {18u, 15u, 9u},
    {18u, 18u, 9u},
    {21u, 18u, 9u},
    {24u, 18u, 12u},
    {24u, 21u, 12u},
};

static const WaveComposition MAP3_WAVES[NORMAL_WAVES] = {
    {12u, 0u, 0u},
    {15u, 15u, 6u},
    {18u, 15u, 9u},
    {21u, 18u, 9u},
    {21u, 21u, 12u},
    {24u, 21u, 12u},
    {27u, 21u, 12u},
};

unsigned waves_match_duration_sec(int level) {
    if (level == 0) return MAP1_DURATION_SEC;
    return MAP23_DURATION_SEC;
}

unsigned waves_paths_for_level(int level) {
    if (level == 0) return 1u;
    if (level == 1) return 3u;
    if (level == 2) return 2u;
    return 1u;
}

unsigned waves_monster_gens_per_path(void) {
    return 3u;
}

unsigned waves_normal_wave_count(void) {
    return NORMAL_WAVES;
}

bool waves_level_has_boss(int level) {
    return level == 1 || level == 2;
}

void waves_get_composition(int level, unsigned wave_index, WaveComposition* out) {
    const WaveComposition* table = MAP1_WAVES;
    if (level == 1)
        table = MAP2_WAVES;
    else if (level == 2)
        table = MAP3_WAVES;

    if (out == NULL) return;
    if (wave_index >= NORMAL_WAVES) {
        out->pawn = 0u;
        out->alien = 0u;
        out->spear = 0u;
        return;
    }
    *out = table[wave_index];
}
