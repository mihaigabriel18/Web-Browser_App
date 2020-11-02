#include "Coada.h"

//initializam o coada cu dimensiunea tuturor elementelor 'dim'
void* InitQ(size_t dim)
{
	AQ queue;
	queue = (AQ)malloc(sizeof(TCoada));
	if (!queue) return NULL;
	//initializari
	DIMEQ(queue) = dim;
	VF(queue) = NULL;
	SF(queue) = NULL;

	return (void*)queue;
}

//Introducem in coada 'queue' o noua celula care contine informatia 'elem'
int IntrQ(void* queue, void* elem)
{
	//alocam o noua celula
	ACeQ aux = (ACeQ)malloc(sizeof(TCellQ));
	if (!aux) return 0; //eroare la alocare
	//alocam informatia
	aux->info = malloc(DIMEQ(queue));
	if (!(aux->info)) { free(aux); return 0; }
	memcpy(aux->info, elem, DIMEQ(queue));
	//facem lagaturile si testam cazurile particulare
	aux->urm = NULL;
	if (VIDAQ(queue))
		SF(queue) = VF(queue) = aux;
	else {
		SF(queue)->urm = aux;
		SF(queue) = aux;
	}
	return 1;
}

//Eliminam un element din coada si il copiem la adresa element
int EXtrQ(void* queue, void* elem)
{
	if (VIDAQ(queue)) return 0; //nu avem ce sa extragem
	//copiem elementul la adresa elem
	memcpy(elem, VF(queue)->info, DIMEQ(queue));
	//mutam pointerii si consideram cazurile particulare
	ACeQ aux = VF(queue);
	if (VIDAQ(queue))
		SF(queue) = VF(queue) = NULL;
	else SF(queue) = aux->urm;
	//eliberam memoria
	free(aux->info);
	free(aux);
	return 1;
}

void ResetQ(void* queue)
{
	if (VIDAQ(queue)) return;
	ACeQ aux = VF(queue);
	ACeQ urm = aux;
	//dezalocam fiecare celula pe rand
	while (urm)
	{
		aux = urm;
		urm = aux->urm;
		free(aux->info);
		free(aux);
	}
	VF(queue) = SF(queue) = NULL;
}