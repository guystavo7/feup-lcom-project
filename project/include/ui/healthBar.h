#ifndef HEALTH_BAR 
#define HEALTH_BAR 

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"

typedef struct HealthBar HealthBar;

HealthBar* healthBarNew(int health);
void healthBarFree(HealthBar* healthBar);
void healthBarUpdate(HealthBar* healthBar, Terminal* terminal, Arena* arena);

int healthBarGetHealth(HealthBar* healthBar);
void healthBarSetHealth(HealthBar* healthBar, int health);
void healthBarAddHealth(HealthBar* healthBar, int health);

#endif
