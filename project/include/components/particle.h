#ifndef PARTICLE 
#define PARTICLE 

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"

#include "../utils/vec2d.h"

typedef struct Particle Particle;

Particle* particleNew(float frameSpeed, SpriteXPM** xpm_array, int n);
void particleFree(Particle* particle);
void particleUpdate(int x, int y, Particle* sprite, Terminal* t, float deltaTime, bool isFlipped);

bool particleIsFinished(Particle* p);
void particleStart(Particle* p);

#endif
