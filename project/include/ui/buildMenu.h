#ifndef BUILD_MENU 
#define BUILD_MENU 

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "../arena.h"
#include "../buildings/genericTower.h"

typedef struct BuildMenu BuildMenu;

BuildMenu* buildMenuNew(TowerKind type);
void buildMenuFree(BuildMenu* buildMenu);
void buildMenuUpdate(BuildMenu* buildMenu, Terminal* terminal, Arena* arena, float deltaTime);
bool buildMenuIsPressed(BuildMenu* buildMenu);
void buildMenuReset(BuildMenu* buildMenu);

#endif
