#ifndef WAVES_H
#define WAVES_H

#include <stdint.h>

#define MAP1_DURATION_SEC 300.0f
#define MAP2_DURATION_SEC 480.0f

typedef enum {
    MONSTER_KIND_PAWN = 0,
    MONSTER_KIND_ALIEN,
    MONSTER_KIND_LANCER,
    MONSTER_KIND_BOSS,
} MonsterKind;


typedef struct {
    float timeSec;
    uint8_t pathIndex;
    MonsterKind kind;
    int count;
} WaveSpawn;

extern const WaveSpawn WAVES_MAP1[];
extern const unsigned int WAVES_MAP1_LEN;

extern const WaveSpawn WAVES_MAP2[];
extern const unsigned int WAVES_MAP2_LEN;

#endif
