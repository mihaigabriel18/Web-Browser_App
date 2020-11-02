#include <stdio.h>
#include <stdlib.h>

typedef struct cell_s
{
	struct cell_s* urm; //celula urmatoare
	void* info; //informatia
}TCellS, *ACeS;

typedef struct 
{
	size_t dime; //dimensiunea unui element
	ACeS vf; //varful stivei
}TStiva, *ASt;

#define DIMES(a) (((ASt)(a))->dime)
#define VARFS(a) (((ASt)(a))->vf)
#define VIDAS(a) (((ASt)(a))->vf == NULL)

void* InitS(size_t dim);
int Push(void* stack, void* elem);
int Pop(void* stack, void* elem);
void ResetS(void* stack, void (*fdel)(void*));
void DistrugereS(void** stack, void (*fdel)(void*));