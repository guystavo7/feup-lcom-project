#include "Vector.h"


#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>





void printTest(const char* format , ...){
#ifdef DEBUG
    va_list args;
    va_start(args,format);
    vprintf(format, args);
    va_end(args);

#endif
}





struct vector {

    char* data;

    unsigned int data_size;
    unsigned int size;
    unsigned int capacity;

} ;


typedef struct vector Vector;

Vector*  vec_create(unsigned int data_size){

    Vector* vec=(Vector*)malloc(sizeof(Vector));
    vec->size=0;
    vec->capacity=1;
    vec->data_size=data_size;
    vec->data= (char*)malloc(data_size*vec->capacity);

    return vec;
}

void  vec_free(Vector* v){

    
    free(v->data);
    free(v);
}

unsigned int vec_size(Vector* v){
    return v->size;
}

void vec_resize(Vector* vec,unsigned int size){

    if(size<0){
        return;
    }


    if (size>vec->capacity){

        while(size>((vec->capacity))){

            vec->capacity*=2; 
        }

        fflush(stdout);
       // vec->data[vec->size]=(char**)malloc(sizeof(char*));
        char* new_data= (char*)malloc(vec->data_size*vec->capacity);

        //memcpy(new_data,vec->data,sizeof(char***)*vec->size-1);

        for(int i=0;i<vec->size*vec->data_size;i++) {

            fflush(stdout);
            new_data[i]=vec->data[i];
        }

        char*temp=vec->data;
        vec->data=new_data;
        free(temp);
    }
    vec->size=size;


}

void vec_push_back(Vector*vec, char* val){ 
    vec_resize(vec,vec->size+1);
    vec_set(vec,(vec->size)-1,val); 
}

void vec_push_back_val(Vector*vec, long long int val){ 
    vec_resize(vec,vec->size+1);
    vec_set_val(vec,(vec->size)-1,val); 
}
/*
void vec_push_mem_back(VECTOR* vec) {
    vec_resize(vec,vec->size+1);
    vec->data[vec->size-1]=(char**)malloc(sizeof(char**));
}
*/
void vec_insert(Vector*vec, unsigned int pos, char* val){


    if( !( 0<=pos and pos<=vec->size)){
        printf("vec insert pos = %d is invalid\n",pos);
        fflush(stdout);
        return;
    }
    vec_resize(vec,vec->size+1);

    for(unsigned int i=vec->size-1; i>pos;i--) {
        for(unsigned int j=0; j<vec->data_size;j++){
            vec->data[i*vec->data_size+j]=vec->data[(i-1)*vec->data_size+j];
        }
    }
    for(unsigned int j=0; j<vec->data_size;j++){
        vec->data[pos*vec->data_size+j]=val[j];
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
void vec_pop_back(Vector*vec) {

    if( vec->size<=0){
        printf("vec cannot pop_back no elements\n");
        fflush(stdout);
        return;
    }

    vec->size--;


}

void vec_remove(Vector*vec, unsigned int pos){

    if( !( 0<=pos and pos<=vec->size)){
        printf("vec insert pos = %d is invalid\n",pos);
        fflush(stdout);
        return;
    }



    for(unsigned int i=pos+1; i< vec->size;i++) {
        for(unsigned int j=0; j<vec->data_size;j++){
            vec->data[(i-1)*vec->data_size+j]=vec->data[i*vec->data_size+j];
        }
        
    }

    vec->size--;


}


void vec_set(Vector*vec, unsigned int pos, char* val){
    if(!(0<=pos && pos<vec->size)){
        printf("index out of range");
        fflush(stdout);
        //error
        return;
        }
    for(unsigned int j=0; j<vec->data_size;j++){
        vec->data[pos*vec->data_size+j]=val[j];
    } 
}

void vec_set_val(Vector*vec, unsigned int pos, long long int val){
    if(!(0<=pos && pos<vec->size)){
        printf("index out of range");
        fflush(stdout);
        //error
        return;
        }
    printTest("set_val-start\n");
    for(unsigned int j=0; j<vec->data_size;j++){
        vec->data[pos*vec->data_size+j]=val;
        val>>=8;
    printTest("val[%d]=%d\n",j,val);

    printTest("vec->data[%d]=%d\n",pos*vec->data_size+j,vec->data[pos*vec->data_size+j]);

    } 
    printTest("set_val-end\n");


}

char* vec_get(Vector*vec, unsigned int pos){
    if(!((0<=pos) && (pos<(vec->size)))){
        printf("index out of range");
        fflush(stdout);
        //error
    } 
    char* val=(char*)malloc(vec->data_size);
    printTest("get-start\n");
    for(unsigned int j=0; j<(vec->data_size);j++){
        val[j]=vec->data[pos*vec->data_size+j]; 
        printTest("val[%d]=%d\n",j,val[j]);

    printTest("vec->data[%d]=%d\n",pos*vec->data_size+j,vec->data[pos*vec->data_size+j]);

    } 
    printTest("get-end\n");

    return val;
}

int vec_find(Vector*vec, char* val) {
    for (int i = 0; i < vec->size; i++) {
        bool equal=true;
        for(unsigned int j=0; j<vec->data_size;j++){
            if(vec->data[i*vec->data_size+j]!=val[j]){
                equal=false;
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