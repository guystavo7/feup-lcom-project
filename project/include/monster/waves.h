#pragma once

#include <stdbool.h>

typedef struct {
    unsigned pawn;
    unsigned alien;
    unsigned spear;
} WaveComposition;

unsigned waves_match_duration_sec(int level);
unsigned waves_paths_for_level(int level);
unsigned waves_monster_gens_per_path(void);

unsigned waves_normal_wave_count(void);

bool waves_level_has_boss(int level);

void waves_get_composition(int level, unsigned wave_index, WaveComposition* out);
