
#include "../../include/data_structures/vector.h"

#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <stdarg.h>

void printTest(const char *format, ...) {
#if DEBUG
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

#endif
}

struct vector {

  char *data;

  unsigned int data_size;
  unsigned int size;
  unsigned int capacity;
};


Vector *vectorCreate(unsigned int data_size) {

  Vector *vec = (Vector *)malloc(sizeof(Vector));
  vec->size = 0;
  vec->capacity = 1;
  vec->data_size = data_size;
  vec->data = (char *)malloc(data_size * vec->capacity);

  return vec;
}

Vector *vectorCreateWithData(unsigned int data_size, unsigned int size, ...) {

  Vector *vec = vectorCreate(data_size);
  

  va_list args;
  va_start(args, size);
  

  for (unsigned int i = 0; i < size; i++) {
    if(data_size<=4){
        int data=(int)va_arg(args,int);
        vectorPushBack(vec,(char*)&data);
    }
    else if(data_size==8){

        long long int data=va_arg(args,long long int);
        vectorPushBack(vec,(char*)&data);
    }
    else{
        printf("unsupported data type");
        fflush(stdout);
    }
  }
  va_end(args);

  return vec;
}

Vector *vectorCreateWithFloatingData(unsigned int data_size, unsigned int size, ...) {

  Vector *vec = vectorCreate(data_size);
  

  va_list args;
  va_start(args, size);
  

  for (unsigned int i = 0; i < size; i++) {
    if(data_size==4){
        float data=va_arg(args,double);
        vectorPushBack(vec,(char*)&data);
    }
    else if(data_size==8 ){
        double data=va_arg(args,double);
        vectorPushBack(vec,(char*)&data);
    }
    else{
        printf("unsupported data type");
        fflush(stdout);
    }
  }
  va_end(args);

  return vec;
}

void vectorFree(Vector *v) {

  free(v->data);
  free(v);
}

unsigned int vectorGetSize(Vector *v) { return v->size; }

void vectorResize(Vector *vec, unsigned int size) {

  
  

  if (size > vec->capacity) {

    while (size > ((vec->capacity))) {

      vec->capacity *= 2;
    }

    fflush(stdout);
    // vec->data[vec->size]=(char**)malloc(sizeof(char*));
    char *new_data = (char *)malloc(vec->data_size * vec->capacity);

    // memcpy(new_data,vec->data,sizeof(char***)*vec->size-1);

    for (unsigned int i = 0; i < vec->size * vec->data_size; i++) {

      fflush(stdout);
      new_data[i] = vec->data[i];
    }

    char *temp = vec->data;
    vec->data = new_data;
    free(temp);
  }
  vec->size = size;
}

void vectorPushBack(Vector *vec, char *val) {
  vectorResize(vec, vec->size + 1);
  vectorSet(vec, (vec->size) - 1, val);
}

void vectorPushBackVal(Vector *vec, long long int val) {
  vectorResize(vec, vec->size + 1);
  vectorSetVal(vec, (vec->size) - 1, val);
}
/*
void vec_push_mem_back(VECTOR* vec) {
    vec_resize(vec,vec->size+1);
    vec->data[vec->size-1]=(char**)malloc(sizeof(char**));
}
*/
void vectorInsert(Vector *vec, unsigned int pos, char *val) {

  if (!( pos <= vec->size)) { //0 <= pos and
    printf("vec insert pos = %d is invalid\n", pos);
    fflush(stdout);
    return;
  }
  vectorResize(vec, vec->size + 1);

  for (unsigned int i = vec->size - 1; i > pos; i--) {
    for (unsigned int j = 0; j < vec->data_size; j++) {
      vec->data[i * vec->data_size + j] =
          vec->data[(i - 1) * vec->data_size + j];
    }
  }
  for (unsigned int j = 0; j < vec->data_size; j++) {
    vec->data[pos * vec->data_size + j] = val[j];
  }
}

/*
void vec_insert_mem(VECTOR*vec, unsigned int pos){

    if( !( 0<=pos and pos<=vec->size)){
        printf("vec insert pos = %d is invalid\n",pos);
        fflush(stdout);
        return;
    }
    vec_resize(vec,vec->size+1);

    for(unsigned int i=vec->size-1; i>pos;i--) {
        vec->data[i]=vec->data[i-1];
    }


    vec->data[pos]=malloc(sizeof(char**));

}
*/
void vectorPopBack(Vector *vec) {

  if (vec->size <= 0) {
    printf("vec cannot pop_back no elements\n");
    fflush(stdout);
    return;
  }

  vec->size--;
}

void vectorRemove(Vector *vec, unsigned int pos) {

  if (!( pos <= vec->size)) {
    printf("vec insert pos = %d is invalid\n", pos);
    fflush(stdout);
    return;
  }

  for (unsigned int i = pos + 1; i < vec->size; i++) {
    for (unsigned int j = 0; j < vec->data_size; j++) {
      vec->data[(i - 1) * vec->data_size + j] =
          vec->data[i * vec->data_size + j];
    }
  }

  vec->size--;
}

void vectorSet(Vector *vec, unsigned int pos, char *val) {
  if (!(pos < vec->size)) {
    printf("index out of range");
    fflush(stdout);
    // error
    return;
  }
  for (unsigned int j = 0; j < vec->data_size; j++) {
    vec->data[pos * vec->data_size + j] = val[j];
  }
}

void vectorSetVal(Vector *vec, unsigned int pos, long long int val) {
  if (!(pos < vec->size)) { //0 <= pos && 
    printf("index out of range");
    fflush(stdout);
    // error
    return;
  }
  printTest("set_val-start\n");

  if (DEBUG) {
    for (unsigned int j = 0; j < vec->data_size; j++) {

      vec->data[pos * vec->data_size + j] = 0;
    }
  }

  memcpy(&vec->data[pos * vec->data_size], &val, vec->data_size);
}

char *vectorGetCopy(Vector *vec, unsigned int pos) {
  if (!( (pos < (vec->size)))) {//(0 <= pos) &&
    printf("index out of range");
    fflush(stdout);
    // error
  }
  char *val = (char *)malloc(vec->data_size);

  printTest("get-start\n");
  for (unsigned int j = 0; j < (vec->data_size); j++) {
    val[j] = vec->data[pos * vec->data_size + j];

    printTest("val[%d]=%d\n", j, val[j]);

    printTest("vec->data[%d]=%d\n", pos * vec->data_size + j,
              vec->data[pos * vec->data_size + j]);
  }
  printTest("get-end\n");

  return val;
}

char *vectorGet(Vector *vec, unsigned int pos) {
  if (!( (pos < (vec->size)))) {
    printf("index out of range");
    fflush(stdout);
    // error
  }
  char *val = &vec->data[pos * vec->data_size];

  return val;
}

long long int vectorGetVal(Vector *vec, unsigned int pos) {
  if (!( (pos < (vec->size)))) {
    printf("index out of range");
    fflush(stdout);
    // error
  }
  long long int val = 0;
  memcpy(&val, &vec->data[pos * vec->data_size], vec->data_size);
  return val;
}

int vectorFind(Vector *vec, char *val) {
  for (unsigned int i = 0; i < vec->size; i++) {
    bool equal = true;
    for (unsigned int j = 0; j < vec->data_size; j++) {
      if (vec->data[i * vec->data_size + j] != val[j]) {
        equal = false;
        break;
      }
    }
    if (equal) {
      return i;
    }
  }
  return -1;
}

/*
int main() {
    VECTOR* v=vec_create();
    for(int i=0;i<10;i++) {

        vec_push_back(v,i);
    }

    for(int i=0;i<10;i++) {
        vec_pop_back(v);
    }
    for(int i=0;i<10;i++) {
        vec_push_back(v,i);

    }
    // 0 ,1 ,...,9





    vec_set(v,8,16);

    //0,...,7,16,9
    vec_remove(v,5);
    //0,..,4,6,7,16,9
    for(int i=0;i<v->size;i++) {
        printf("vec[%d] = %d\n",i,vec_get(v,i));
    }






    return 0;
}*/
