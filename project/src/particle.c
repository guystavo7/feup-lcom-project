#include "../include/components/particle.h"
#include <stdint.h>
#include <stdlib.h>

struct Particle {
    SpriteXPM** sprites;
    int n;

    float frameSpeed;
    float current;
    bool active;
    bool disapearAfterFinished;
};

Particle* particleNew(float frameSpeed, SpriteXPM** xpm_array, int n, bool disapearAfterFinished) {
    Particle* res = malloc(sizeof(Particle) * n);
    res->sprites = xpm_array;
    res->frameSpeed = frameSpeed;
    res->current = 0;
    res->n = n;
    res->active = false;
    res->disapearAfterFinished = disapearAfterFinished;

    return res;
}

void particleFree(Particle* particle) {
    for (int i = 0; i < particle->n; i++) freeSpriteXPM(particle->sprites[i]);
    free(particle->sprites);
    free(particle);
}

void particleUpdate(int x, int y, Particle* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    if (sprite->active) {
        sprite->current += deltaTime * sprite->frameSpeed;

        if (sprite->current >= sprite->n) {
            sprite->active = false;
            sprite->current = sprite->n - 1;
        }
    }

    if (sprite->active || !sprite->disapearAfterFinished) {
        SpriteXPM* currentFrame = sprite->sprites[(int)sprite->current];
        drawSpriteXPM(currentFrame, t, x, y, isFlipped);
    }
}

bool particleIsFinished(Particle* p) {
    return !p->active;
}

void particleStart(Particle* p) {
    p->active = true;
    p->current = 0;
}

void particleReset(Particle* p) {
	p->current = 0;
}
