#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "Stiva.h"
#include "Coada.h"
#include "DoubleLinkedList.h"

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_

typedef struct {
	char name[100];
	unsigned long dimension;
	unsigned long currently_downloaded;
} Resource;

Resource* get_page_resources(const char* URL_Name, int* n);

#endif /* _DOWNLOAD_RESOURCE_ */

#define wait(a) (Sleep(a))

//structura pentru pagina web
typedef struct {
	char url[21];
	unsigned int num_res; //nuamr de resurse
	Resource* resources; //vector de resurse
} WebPage, *A_WebPage;

//structura pentru un tab
typedef struct {
	WebPage* current_page;
	void* back_stack, * forward_stack;
} Tab, *A_Tab;

void changeBandwidth(int value, int* bandwidth);
A_Tab creareTabNou();
void addNewTab(TLDI tabs);
void delEndTab(TLDI tabs, TLDI* currTab);
void changeTab(TLDI tabs, TLDI* currTab, int n);
void printOpenTabs(TLDI tabs);
void goToUrl(TLDI currTab, char* nume, void* historyQ);
void goBack(TLDI currTab);
void goForward(TLDI currTab);
void printHistory(void* historyQ);
void delEntries(void* historyQ, int n);