#include "../include/waves.h"

float waveSpawnScheduledTime(const WaveSpawn* w, int k) {
    int n = w->count;
    if (n <= 1) {
        return w->timeSec;
    }
    if (k <= 0) {
        return w->timeSec;
    }
    if (k >= n) {
        k = n - 1;
    }
    {
        float spacing;
        if (w->durationSec > 0.0f) {
            spacing = w->durationSec / (float)(n - 1);
        } else {
            spacing = WAVE_DEFAULT_DRIP_SEC;
        }
        return w->timeSec + spacing * (float)k;
    }
}
