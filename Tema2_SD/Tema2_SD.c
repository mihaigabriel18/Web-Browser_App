#include "utils.h"

#define NR_COMENZI 14 //avem 14 comenzi posibile 
#define MAX_DIM_COMMAND 40 //o comanda are maxim 40 de caractere
#define MAX_CUV_COMMAND 2 //o comanda are maxim 2 cuvinte

TLDI initialState(int* bandwidth, TLDI* tabs)
{
	*bandwidth = 1024;
	size_t tab_size = sizeof(Tab);
	*tabs = InitLDI(tab_size);
	//cream primul tab care nu contine inca nimic
	addNewTab(*tabs);
	return (*tabs)->urm; //returnam primul tab ca fiind cel curent
}

/*
functie care creaza paternurile de comenzi
retinand primul cuvant din fiecare comanda
*/
char** createCommands()
{
	char** comenzi = (char**)malloc(NR_COMENZI * sizeof(char*));
	if (!comenzi) return NULL; //esuare la alocare
	int i = 0;
	for (i = 0; i < NR_COMENZI; i++)
	{
		comenzi[i] = (char*)calloc(MAX_DIM_COMMAND, sizeof(char));
		if (!comenzi[i]) //esuare la alocare
		{
			//trebuie intai sa dezalocam tot ce e intainte de comenzi[i]
			int j;
			for (j = 0; j < i; j++)
				free(comenzi[j]);
			free(comenzi);
			return NULL;
		}
	}
	//le asociem un nume
	strcat(comenzi[0], "set_band");
	strcat(comenzi[1], "newtab");
	strcat(comenzi[2], "deltab");
	strcat(comenzi[3], "change_tab");
	strcat(comenzi[4], "print_open_tabs");
	strcat(comenzi[5], "goto");
	strcat(comenzi[6], "back");
	strcat(comenzi[7], "forward");
	strcat(comenzi[8], "history");
	strcat(comenzi[9], "del_history");
	strcat(comenzi[10], "list_dl");
	strcat(comenzi[11], "downloads");
	strcat(comenzi[12], "download");
	strcat(comenzi[13], "wait");
	return comenzi;
}

/*
fuctie care stie sa apeleze functiile din 'utils.c' in functie de indice
primeste ca parametrii si al doilea cuvant deoarece aceste este o valoare
pentru unele functii iar pentru celelalte este NULL si nu este folosit
mai primeste ca parametrii si bandwidth ul, tabul curent, lista de taburi
si coada de istoric global
*/
void functionCall(int index, char* secondWord, int* bandwidth,
				TLDI* currTab, TLDI tabs, void* historyQ)
{
	switch (index)
	{
	case 0: //set_bandwidth function
		changeBandwidth(atoi(secondWord), bandwidth);
		break;
	case 1: //newtab function
		addNewTab(tabs);
		break;
	case 2: //deltab function
		delEndTab(tabs, currTab);
		break;
	case 3: //change_tab function
		changeTab(tabs, currTab, atoi(secondWord));
		break;
	case 4: //print_open_tabs function
		printOpenTabs(tabs);
		break;
	case 5: //goto function
		goToUrl(*currTab, secondWord, historyQ);
		break;
	case 6: //back function
		goBack(currTab);
		break;
	case 7: //forward function
		goForward(currTab);
		break;
	case 8: //history function
		printHistory(historyQ);
		break;
	case 9: //del_history function
		delEntries(historyQ, atoi(secondWord));
		break;
	case 10: //list_dl function
		break;
	case 11: //downloads function
		break;
	case 12: //download function
		break;
	case 13: //wait function
		wait(atoi(secondWord));
		break;
	}
}

//functie care primeste comenzi de la STDIN si apeleaza functiile necesare
void primireComenzi(char** comenziPosibile, int* bandwidth,
					TLDI* currTab, TLDI tabs, void* historyQ, FILE* file)
{
	char* comanda = (char*)malloc(MAX_DIM_COMMAND * sizeof(char));
	if (!comanda) return;
	//citim comenzi
	while (fgets(comanda, MAX_DIM_COMMAND, file))
	{
		char* firstWord = (char*)malloc(MAX_DIM_COMMAND * sizeof(char));
		if (!firstWord) return; //eroare la alocare
		char* secondWord = (char*)malloc(MAX_DIM_COMMAND * sizeof(char));
		if (!secondWord) return; //eroare la alocare
		firstWord = strtok(comanda, " "); //primul cuvant din comanda
		secondWord = strtok(NULL, " "); //este null daca nu avem 2 cuvinte
		/*
		veriricam care comanda din sirul 'comenziPosibile' trebuie apelata
		prin a obtine un indice catre sirul de caractere cu care se potriveste
		*/
		int i;
		for (i = 0; i < NR_COMENZI; i++)
		{
			//printf("%d\n%d\n", firstWord[6], comenziPosibile[i][6]);
			if (!strcmp(firstWord, comenziPosibile[i]))
				break;
		}
		/*
		'i' la momentul de fata reprezinta numarul functiei care trebuie
		apelata, astfel vom apela o functie care stie ca sa apeleze in
		functie de ce indice ii trimitem
		*/
		if (i == NR_COMENZI) printf("Comanda nu exsita\n");
		functionCall(i, secondWord, bandwidth, currTab, tabs, historyQ);
	}
}

int main(int argc, char* argv[])
{
	void* historyQ = InitQ(20*sizeof(char)); //coada de url-uri
	int bandwidth;
	TLDI tabs = NULL, currTab;
	//setam tabul curent la primul creat si setam starile initiale
	currTab = initialState(&bandwidth, &tabs);
	FILE* in = fopen("input.txt", "r");
	//creem un vector de comenzi posibile
	char** comenzi = createCommands();
	primireComenzi(comenzi, &bandwidth, &currTab, tabs, historyQ, in);
}