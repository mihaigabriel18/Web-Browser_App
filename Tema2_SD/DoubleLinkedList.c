#include "DoubleLinkedList.h"

//initializam lista dublu inlantuita si santinela sa cu dimensiunea 'dim'
TLDI InitLDI(size_t dim)
{
	TLDI aux = (TLDI)malloc(sizeof(TCell2));
	if (!aux)
		return NULL;
	//creem santinela si retinem la informatia santinelei dimensiunea
	//unui element din aceasta lista
	aux->info = (size_t*)malloc(sizeof(size_t));
	if (!aux->info) { free(aux); return NULL; }
	aux->pre = aux->urm = aux;
	memcpy(aux->info, &dim, sizeof(dim));
	return aux;
}

//alocam o noua celula cu inforamtia elem de dimensiune 'dimElem'
TLDI allocCell(void* elem, size_t dimElem)
{
	TLDI aux = (TLDI)malloc(sizeof(TCell2));
	if (!aux) return NULL;
	//alocam informatia
	aux->info = malloc(dimElem);
	if (!(aux->info)) { free(aux); return NULL; }
	memcpy(aux->info, elem, dimElem);
	return aux;
}

//inseram la sfarsit in lista un element care contine informatia 'elem'
int InserareSfLDI(TLDI list, void* elem)
{
	TLDI aux = allocCell(elem, DIMEL(list));
	//cazul in care a esuat alocarea
	if (!aux) return 0;
	//legam celula la lista noastra
	list->pre->urm = aux;
	aux->urm = list;
	return 1;
}

//elimniam ultima celula a listei cu ajutorul functiei de stergere 'fdel'
int EliminareSfLDI(TLDI list)
{
	if (VIDAL(list)) return 0;
	TLDI aux = list->pre;
	aux->pre->urm = list;
	free(aux->info);
	free(aux);
}