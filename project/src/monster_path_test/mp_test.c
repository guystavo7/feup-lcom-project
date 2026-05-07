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

static Vector* listOfMonsters;

static int initial_value=1;
static float counter;



 void startGame() {
    counter=initial_value;

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

    listOfMonsters=vectorCreate(monsterSizeof());

    

    vectorPushBack(listOfMonsters,(char*)monsterCreate(0,0,100,0));

    pathSubscribe(path,(void *)vectorGetSize(listOfMonsters));



    
}

void exitGame() {
    freeSpriteXPM(sprite);
    
    vectorFree(listOfMonsters);
    pathFree(path);
}

void gameLoop(Terminal* terminal,float delta_time) {
    
    counter-=delta_time;

    if(counter<0 && vectorGetSize(listOfMonsters)<=1000){
        counter=initial_value;
        printf("new_monster!\n");
        vectorPushBack(listOfMonsters,(char*) monsterCreate(0,0,100,0));
        pathSubscribe(path,(void *)vectorGetSize(listOfMonsters));
    }
    

    Vec2d* n_dir=vec2dCreate(0,0);
    Vec2d* pos=vec2dCreate(0,0);
    for(unsigned int i=0;i<vectorGetSize(listOfMonsters);i++){
        
        Monster* m=(Monster*)vectorGet(listOfMonsters,i);
        monsterGetPos(m,pos);
        pathFollowingDir(path,(void *)(i+1),pos,n_dir);
        monsterSetDirWithVec(m,n_dir);

        monsterUpdatePos(m,delta_time);

        drawSpriteXPM(sprite, terminal, monsterGetX(m), monsterGetY(m));
        
    }
    vec2dFree(pos);
    vec2dFree(n_dir);
}
