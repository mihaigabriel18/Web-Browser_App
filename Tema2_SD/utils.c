#include "utils.h"

Resource* get_page_resources(const char* URL_Name, int* n) {
	int i, j, name_len, url_name_len = strlen(URL_Name);

	unsigned int resource_nr = 0;
	uint32_t size = 0;
	if (URL_Name == NULL) {
		return NULL;
	}

	for (i = 0; i < url_name_len; i++) {
		resource_nr += URL_Name[i];
	}

	resource_nr %= 13;
	Resource* result = (Resource*)calloc(resource_nr, sizeof(Resource));
	*n = resource_nr;
	for (i = 0; i < resource_nr; i++) {
		sprintf(result[i].name, "%s-(%d)", URL_Name, i);
		size = 0;
		name_len = strlen(result[i].name);

		for (j = 0; j < name_len; j++) {
			size += result[i].name[j];
		}
		/* Some randomness */
		size ^= size << 3;
		size += size >> 5;
		size ^= size << 4;
		size += size >> 17;
		size ^= size << 25;
		size += size >> 6;

		/* 100MB Maximum size */
		result[i].dimension = size % 104857601;
		result[i].currently_downloaded = 0;
	}

	return result;
}

//functie care stie sa stearga o pagina web
void delWebPage(void* info)
{
	A_WebPage aux = (A_WebPage)info;
	free(aux->resources);
	free(aux);
}

//primeste o valoare intreaga si modifica valoarea bandwidth-ului
void changeBandwidth(int value, int *bandwidth)
{
	*bandwidth = value;
}

//functie care creaza un tab nou
A_Tab creareTabNou()
{
	A_Tab newTab = (A_Tab)malloc(sizeof(Tab));
	if (!newTab) return NULL;
	//initializam campurile
	newTab->current_page = NULL;
	newTab->back_stack = InitS(sizeof(WebPage));
	newTab->forward_stack = InitS(sizeof(WebPage));
	return newTab;
}

//inseram un tab gol la sfarsit
void addNewTab(TLDI tabs)
{
	A_Tab newTab = creareTabNou();
	InserareSfLDI(tabs, newTab);
}

//stergem toate informatiile dintr-un tab
void delTab(A_Tab tab)
{
	free(tab->current_page->resources);
	free(tab->current_page);
	DistrugereS(tab->back_stack, delWebPage);
	DistrugereS(tab->forward_stack, delWebPage);
}

//stergem tabul de la sfarsit si mutam pozitia tabului curent
void delEndTab(TLDI tabs, TLDI* currTab)
{
	//stergem informatiile din ultimul tab
	delTab((A_Tab)(tabs->pre->info));
	//verificam daca tabul curent este cel pe care vrem sa il stergem
	if (*currTab == tabs->pre)
		* currTab = tabs->pre->pre;
	EliminareSfLDI(tabs);
}

//mutam pozitia tabului curent la al 'n'-lea tab
void changeTab(TLDI tabs, TLDI* currTab, int n)
{
	int i = 0;
	for (*currTab = tabs->urm; i < n; *currTab = (*currTab)->urm, i++);
}

//afisam taburile deschise
void printOpenTabs(TLDI tabs)
{
	int i;
	TLDI aux;
	for (aux = tabs->urm, i = 0; aux != tabs; aux = aux->urm, i++)
	{
		A_Tab currTab = (A_Tab)aux->info;
		char* url = currTab->current_page->url;
		if(url)
			printf("(%d: %s)\n", i, currTab->current_page->url);
		else 
			printf("(%d: empty)\n", i);
	}
}

//alocarea unei pagini web pe baza numelui acesteia
A_WebPage allocPagina(char* nume)
{
	A_WebPage aux = (A_WebPage)malloc(sizeof(WebPage));
	if (!aux) return NULL;
	//ii punem numele
	strcat(aux->url, nume);
	int n;
	//generam resursele
	Resource* tempRes = get_page_resources(nume, &n);
	aux->num_res = n;
	aux->resources = tempRes;
	return aux;
}

//accesam o adresa web
void goToUrl(TLDI currTab, char* nume, void* historyQ)
{
	A_Tab curr = (A_Tab)(currTab->info);
	//punem pagina curenta in stiva de back daca nu e NULLa
	if (curr->current_page)
		Push(curr->back_stack, curr->current_page);
	//modificam pagina curenta
	curr->current_page = allocPagina(nume);
	//resetam stiva de forward
	ResetS(curr->forward_stack, delWebPage);
	//adaugam noua pagina in istoric
	IntrQ(historyQ, nume);
	//in final asteptam o secunda
	wait(1);
}

//ne intoarcem la ultiam pagina din stiva de back
void goBack(TLDI currTab)
{
	A_Tab curr = (A_Tab)(currTab->info);
	//verificam daca este posibila operatia de back
	if (VIDAS(curr->back_stack))
	{
		printf("can't go back, no pages in stack\n");
		return;
	}
	//mutam pagina curenta in forward
	Push(curr->forward_stack, curr->current_page);
	//extragem din stiva de back si punem in pagina curenta
	Pop(curr->back_stack, curr->current_page);
}

//ne ducem la urmatoarea pagina (prima din stiva de forward)
void goForward(TLDI currTab)
{
	A_Tab curr = (A_Tab)(currTab->info);
	//verificam daca este posibila operatia de forward
	if (VIDAS(curr->forward_stack)) 
	{ 
		printf("can't go forward, no pages in stack\n"); 
		return;
	}
	//mutam pagina curenta in back
	Push(curr->back_stack, curr->current_page);
	//extragem din stiva de forward si punem in pagina curenta
	Pop(curr->forward_stack, curr->current_page);
}

//afiseaza intregul istoric
void printHistory(void* historyQ)
{
	ACeQ aux;
	for (aux = VF(historyQ); aux; aux = aux->urm)
	{
		char* URL = *((char**)(aux->info));
		printf("%s\n", URL);
	}
}

//stergem primele n intrari din istoric
void delEntries(void* historyQ, int n)
{
	//verificam daca n = 0 sa stergem toate intrarile
	if (n == 0)
		ResetQ(historyQ);
	int i;
	char* junk = (char*)malloc(21 * sizeof(char));
	for (i = 0; i < n || VIDAQ(historyQ); i++);
		EXtrQ(historyQ, junk);
}