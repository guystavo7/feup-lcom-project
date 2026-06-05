#include "../include/waves.h"

const WaveSpawn WAVES_MAP1[] = {
    {0.0f, 8.0f, 0, MONSTER_KIND_PAWN, 2},
    {25.0f, 10.0f, 0, MONSTER_KIND_PAWN, 3},
    {55.0f, 0.0f, 0, MONSTER_KIND_ALIEN, 1},
    {90.0f, 14.0f, 0, MONSTER_KIND_PAWN, 4},
    {120.0f, 6.0f, 0, MONSTER_KIND_ALIEN, 2},
    {160.0f, 0.0f, 0, MONSTER_KIND_LANCER, 1},
    {200.0f, 18.0f, 0, MONSTER_KIND_PAWN, 5},
    {240.0f, 7.0f, 0, MONSTER_KIND_ALIEN, 2},
    {270.0f, 9.0f, 0, MONSTER_KIND_LANCER, 2},
};

const unsigned int WAVES_MAP1_LEN = (unsigned int)(sizeof(WAVES_MAP1) / sizeof(WAVES_MAP1[0]));
