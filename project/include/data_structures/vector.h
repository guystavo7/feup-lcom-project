
#define DEBUG 0

typedef struct vector Vector;

Vector*  vectorCreate(unsigned int data_size);

Vector *vectorCreateWithData(unsigned int data_size, unsigned int size, ...);
Vector *vectorCreateWithFloatingData(unsigned int data_size, unsigned int size, ...);
void  vectorFree(Vector* v);

unsigned int vectorGetSize(Vector* v);

void vectorResize(Vector* vec,unsigned int size);

void vectorPushBack(Vector*vec, char* val);
void vectorPushBackVal(Vector*vec, long long int val);

void vectorInsert(Vector*vec, unsigned int pos, char* val);

void vectorPopBack(Vector*vec);
void vectorRemove(Vector*vec, unsigned int pos);

void vectorSet(Vector*vec, unsigned int pos, char* val);
void vectorSetVal(Vector*vec, unsigned int pos, long long int val);
char* vectorGet(Vector*vec, unsigned int pos);
long long int vectorGetVal(Vector*vec, unsigned int pos);
char* vectorGetCopy(Vector*vec, unsigned int pos);
int vectorFind(Vector*vec, char* val);