//
//  fileProcess.h
//  
//
//  Created by Guy on 2/22/15.
//
//

#include "definitions.h"

typedef struct Node
{
    char *name;
    unsigned long int address;
    struct Node *next;
} Symbol;

typedef struct
{
    Symbol *head;
} SymbolList;

