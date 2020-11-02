#include "Stiva.h"

//initializeaza stiva cu elementele de dimensiune 'dim'
void* InitS(size_t dim)
{
	ASt stack;
	stack = (ASt)malloc(sizeof(TStiva));
	if (!stack) return NULL;
	//initializari
	DIMES(stack) = dim;
	VARFS(stack) = NULL;

	return (void*)stack;
}

//introduce un element in varful stivei si returneaza 1 pentru reusita
//si 0 pentru eroare
int Push(void* stack, void* elem)
{
	//alocam o noua celula
	ACeS aux = (ACeS)malloc(sizeof(TCellS));
	if (!aux) return 0; //eroare la alocare
	//alocam informatia
	aux->info = malloc(DIMES(stack));
	if (!(aux->info)) { free(aux);  return 0; }
	//copaim elementul de la elem
	memcpy(aux->info, elem, DIMES(stack));
	//lipim celula in varful stivei
	aux->urm = VARFS(stack);
	VARFS(stack) = aux;
	return 1;
}

//extragem elemenul din varful stivei si il copiem la adresa 'elem'
//returnam 1 pentru succes si 0 pentru eroare
int Pop(void* stack, void* elem)
{
	//analizam cazul in care stiva e vida
	if (VIDAS(stack)) return  0;
	//copiem elementul la adresa elem
	memcpy(elem, VARFS(stack)->info, DIMES(stack));
	//retinem un pointer la elementul pe care il extragem
	ACeS aux = VARFS(stack);
	VARFS(stack) = aux->urm;
	//eliberam memoria
	free(aux->info);
	free(aux);
	return 1;
}

//resetam stiva facand ca varful sa pointeze la NULL
void ResetS(void* stack, void (*fdel)(void*))
{
	//daca e vida teriminam direct
	if (VIDAS(stack)) return;
	ACeS aux = VARFS(stack);
	ACeS urm = aux;
	//dezalocam fiecare celula pe rand
	while (urm)
	{
		aux = urm;
		urm = aux->urm;
		fdel(aux->info);
		free(aux);
	}

	VARFS(stack) = NULL;
}

//eliberam toata memoria stivei, unde stack este un pointer la o stiva
void DistrugereS(void** stack, void (*fdel)(void*))
{
	ResetS(*stack, fdel);
	free(stack);
	*stack = NULL;
}	