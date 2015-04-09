
#include "definitions.h"

SymbolList * symbols_list(My_File *);

int increaseBy (My_Line *);

void free_list (SymbolList *);

SymbolList * new_symbolList();

Symbol * new_symbol(char *, int);

Symbol *search_list(SymbolList *, char *);
