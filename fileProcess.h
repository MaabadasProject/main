//
//  fileProcess.h
//  
//
//  Created by Guy on 2/22/15.
//
//

#include "definitions.h"

#define MAX_SYMBOL_NAME 30

typedef struct Node
{
    char name[MAX_SYMBOL_NAME + 1]; /* +1 for '\0' */
    unsigned long int address;
    struct Node *next;
} Symbol;

typedef struct
{
    Symbol *head;
} SymbolList;

int get_opcode(void);

SymbolList symbol_list (FILE *);

SymbolList * new_symbolList();

Symbol * new_symbol(char *, unsigned long int);

void free_list (SymbolList);

Symbol * get_symbol_name (FILE *, long, SymbolList);
