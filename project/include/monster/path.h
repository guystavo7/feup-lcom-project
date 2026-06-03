#pragma once
#include "../data_structures/vector.h"
#include "./../utils/vec2d.h"

typedef struct path Path;


Path* pathNew(float point_change_distance,Vector* points);//points parameter must be a 4-byte-data-size vector with a even number (different of 0) of floats that correspont to pairs (x,y). point_change_distance should be>0

void pathFree(Path* p);

void pathSubscribe(Path* p,void* pointer);

void pathGoToNextPoint(Path* p,void* pointer);
void pathGetCurPoint(Path* p,void* pointer, Vec2d* curPoint);//curPoint should be initialized


bool pathCheckInPointChangeDistance(Path* p,void* pointer,Vec2d* pos);
void pathGetDirectionToCurPoint(Path* p,void* pointer,Vec2d* pos,Vec2d* new_dir);//new_dir should be initialized


void pathFollowingDir(Path* p,void* pointer,Vec2d* pos,Vec2d* new_dir);//new_dir should be initialized

unsigned int pathGetPointsSize(Path* path);

void pathGetPoint(Path* p,unsigned int i,Vec2d* point);
     

 

unsigned int pathGetNumOfPoints(Path* path);

unsigned int pathGetCurPointIndex(Path* p, void* pointer);
