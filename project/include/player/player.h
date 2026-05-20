#include "./../utils/vec2d.h"
#include "./../arena.h"

typedef struct Player Player;

Player* playerNew(Vec2d pos);
void playerFree(Player* player);
void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime);
