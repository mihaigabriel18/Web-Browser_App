#include <stdio.h>
#include <stdlib.h>

typedef struct cellc {
	struct cellc* urm;
	void* info;
}TCellQ, *ACeQ;

typedef struct {
	size_t dime; //dimensiunea unui element din coada
	ACeQ vf, sf; //pointeri la inceputul cozii si la sfarsitul acesteia
} TCoada, *AQ;

//macrodefinitii utile
#define VIDAQ(a) (((AQ)(a))->vf == NULL && ((AQ)(a))->sf == NULL)
#define DIMEQ(a) (((AQ)(a))->dime)
#define VF(a) (((AQ)(a))->vf)
#define SF(a) (((AQ)(a))->sf)

//functii de prelucrare cozi
void* InitQ(size_t dim);
int IntrQ(void* queue, void* elem);
int EXtrQ(void* queue, void* elem);
void ResetQ(void* queue);