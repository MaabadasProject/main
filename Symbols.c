
#include "Symbols.h"

SymbolList symbols_list(My_File file)
{
    SymbolList list;
    
    &list = new_symbolList();
}

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
Symbol * new_symbol(char *symName, long symValue)
{
    /* TODO: write when it's time */
	Symbol *p;
	p = (Symbol *) malloc (sizeof(Symbol *));
	p->name = symName; // maybe copy it instead of rereferencing it?
	p->value = symValue;
	p->next = NULL;
	return p;
}
