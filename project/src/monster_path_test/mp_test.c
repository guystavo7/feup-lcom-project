#include "../../include/monster_path_test/mp_test.h"
#include "../../../lab5/spriteXMP.h"
#include "../../include/inputHandler.h"
#include "../../resources/1.h"
#include "../../include/data_structures/vector.h"
#include "../../include/monster_path_test/monster.h"
#include "../../include/monster_path_test/path.h"

#include <stdio.h>

static SpriteXPM* sprite;
static Vector* pathPoints;
static Path* path;
static Monster* monster;


void startGame() {
    sprite = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8);
    //pathPoints=vectorCreateWithFloatingData(4,4*2,10.0f,10.0f,450.0f,10.0f,10.0f,450.0f,450.0f,450.0f);
    pathPoints = vectorCreateWithFloatingData(sizeof(float), 11 * 2, 
    400.0f, 50.0f,    // 1.  Ponta Topo
    456.1f, 222.7f,   // 2.  Vale Superior Direito
    637.8f, 222.7f,   // 3.  Ponta Direita
    490.8f, 329.5f,   // 4.  Vale Inferior Direito
    546.9f, 502.2f,   // 5.  Ponta Inferior Direita
    400.0f, 395.5f,   // 6.  Vale Central Inferior
    253.1f, 502.2f,   // 7.  Ponta Inferior Esquerda
    309.2f, 329.5f,   // 8.  Vale Inferior Esquerdo
    162.2f, 222.7f,   // 9.  Ponta Esquerda
    343.9f, 222.7f,   // 10. Vale Superior Esquerdo
    400.0f, 50.0f     // 11. Ponta Topo (Fecha o caminho)
);
    path=pathCreate(3.0f,pathPoints);
    monster=monsterCreate(0,0,100,0);
    pathSubscribe(path,monster);
}

void exitGame() {
    freeSpriteXPM(sprite);
}

void gameLoop(Terminal* terminal,float delta_time) {
    
    Vec2d* n_dir=vec2dCreate(0,0);
    Vec2d* pos=vec2dCreate(0,0);
    monsterGetPos(monster,pos);
    pathFollowingDir(path,&monster,pos,n_dir);
    monsterSetDirWithVec(monster,n_dir);

    monsterUpdatePos(monster,delta_time);

    drawSpriteXPM(sprite, terminal, monsterGetX(monster), monsterGetY(monster));
}
