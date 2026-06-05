#ifndef WAVES_H
#define WAVES_H

#include <stdint.h>

#include "monster/monsterKind.h"

#define MAP1_DURATION_SEC 300.0f
#define MAP2_DURATION_SEC 480.0f


#define WAVE_DEFAULT_DRIP_SEC 0.5f

typedef struct {
    float timeSec;
    float durationSec;
    uint8_t pathIndex;
    MonsterKind kind;
    int count;
} WaveSpawn;

float waveSpawnScheduledTime(const WaveSpawn* w, int k);

extern const WaveSpawn WAVES_MAP1[];
extern const unsigned int WAVES_MAP1_LEN;

extern const WaveSpawn WAVES_MAP2[];
extern const unsigned int WAVES_MAP2_LEN;

#endif
