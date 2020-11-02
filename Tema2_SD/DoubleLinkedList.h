#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
	struct cell* pre, * urm;
	void* info;
} TCell2, *TLDI;

#define DIMEL(a) (*((size_t*)((a)->info)))
#define VIDAL(a) ((a)->urm == (a))

TLDI InitLDI(size_t dim);
int InserareSfLDI(TLDI list, void* elem);
int EliminareSfLDI(TLDI list);