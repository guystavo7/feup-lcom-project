#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stdbool.h>
#include <stdarg.h>
#pragma once

#define EPSILON 0.0001f

typedef struct vec2d Vec2d;

Vec2d* vec2dCreate(float x,float y);

Vec2d* vec2dCreateWithVec2d(Vec2d* v2);

void vec2dFree(Vec2d* v);

float vec2dGetX(Vec2d* v);
float vec2dGetY(Vec2d* v);

void vec2dSetX(Vec2d* v,float x);
void vec2dSetY(Vec2d* v,float y);


void vec2dSetWithVec2d(Vec2d* v,Vec2d* v2);

float vec2dModule(Vec2d* v);

float vec2dModule2(Vec2d* v);


void vec2dNormalize(Vec2d* v);

void vec2dMultiply(Vec2d* v,float delta);

void vec2dAddVec(Vec2d*v,Vec2d* v2);

void vec2dSubVec(Vec2d*v,Vec2d* v2);
