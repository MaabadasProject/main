
#include "Symbols.h"
#include "definitions.h"

int get_header(My_Line *);

unsigned short get_parameter(parameter *, SymbolList  *, int, unsigned int);

void writeObjLine(FILE *, My_Line *, SymbolList *, unsigned int *);

void writeExLine (FILE *, My_Line *, SymbolList *, unsigned int *);

void writeEnLine (FILE *, My_Line *, SymbolList *);

void makeObject(My_File *, SymbolList *, char *);

void makeExtern(My_File *, SymbolList *, char *);

void makeEntry(My_File *, SymbolList *, char *);
