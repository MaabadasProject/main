
#include "definitions.h"

char * assemblyFile(char *);

void processFile (FILE *, char *);

My_File * new_file(FILE *);

SymbolList * symbols_list(My_File *);

void free_list(SymbolList *);

void free_file(My_File *);

int check_direct_variables (My_File *, SymbolList *);

void makeObject(My_File * , SymbolList *, char *);

void makeExtern(My_File * , SymbolList *, char *);

void makeEntry(My_File * , SymbolList *, char *);

void printErrors(My_File *);

Symbol * search_list(SymbolList *, char *);
