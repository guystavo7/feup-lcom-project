#ifndef STATS_BAR 
#define STATS_BAR 

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"

typedef struct StatsBar StatsBar;

StatsBar* statsBarNew();
void statsBarFree(StatsBar* healthBar);
void statsBarUpdate(StatsBar* statsBar, Terminal* terminal, Arena* arena);

int statsBarGetBanana(StatsBar* statsBar);
int statsBarGetAvocado(StatsBar* statsBar);
int statsBarGetCherry(StatsBar* statsBar);
int statsBarGetCarrot(StatsBar* statsBar);

void statsBarSetBanana(StatsBar* statsBar, int n);
void statsBarSetAvocado(StatsBar* statsBar, int n);
void statsBarSetCherry(StatsBar* statsBar, int n);
void statsBarSetCarrot(StatsBar* statsBar, int n);

void statsBarAddBanana(StatsBar* statsBar, int n);
void statsBarAddAvocado(StatsBar* statsBar, int n);
void statsBarAddCherry(StatsBar* statsBar, int n);
void statsBarAddCarrot(StatsBar* statsBar, int n);


#endif
