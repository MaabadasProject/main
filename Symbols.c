//
//  Symbols.c
//  
//
//  Created by Yaki on 2/23/15.
//
//

#include "Symbols.h"

/* this will free the symbols list */
void free_list (SymbolList list)
{
    if (list.head)
    {
        Symbol *curr, *post;
        curr = list.head;
        post = curr->next;
        while (post)
        {
            free(curr);
            curr = post;
            post = post->next;
        }
        free(curr);
    }
    free(list);
}

/* returns a pointer to a new SymbolList created using malloc */
SymbolList * new_symbolList()
{
    SymbolList *list;
    list = (SymbolList *)malloc(sizeof(SymbolList));
    list->head = NULL;
    return list;
}

/* returns a pointer to a new Symbol created using malloc */
/* symName should by shorter than 31 chars */
Symbol * new_symbol(char *symName, long symAddress)
{
    Symbol *sym;
    sym = (Symbol *)malloc(sizeof(Symbol));
    strncpy(sym->name,symName,MAX_SYMBOL_NAME);
    sym->name[MAX_SYMBOL_NAME+1] = '\0';
    sym->address = symAddress;
    sym->next = NULL;
    return sym;
}
