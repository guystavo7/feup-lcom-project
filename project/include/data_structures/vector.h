
#pragma once
#define DEBUG 0

typedef struct vector Vector;

Vector*  vec_create(unsigned int data_size);

void  vec_free(Vector* v);

unsigned int vec_size(Vector* v);

void vec_resize(Vector* vec,unsigned int size);

void vec_push_back(Vector*vec, char* val);
void vec_push_back_val(Vector*vec, long long int val);

void vec_insert(Vector*vec, unsigned int pos, char* val);

void vec_pop_back(Vector*vec);
void vec_remove(Vector*vec, unsigned int pos);

void vec_set(Vector*vec, unsigned int pos, char* val);
void vec_set_val(Vector*vec, unsigned int pos, long long int val);
char* vec_get(Vector*vec, unsigned int pos);
long long int vec_get_val(Vector*vec, unsigned int pos);
char* vec_get_copy(Vector*vec, unsigned int pos);
int vec_find(Vector*vec, char* val);