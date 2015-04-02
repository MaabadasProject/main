
#include "Symbols.h"

SymbolList * symbols_list(My_File *file)
{
    SymbolList *list;
    Symbol *currSym, *dummy;
    My_Line *currLine;
    int wordCount;
    
    list = new_symbolList();
    dummy->next = NULL;
    currSym = dummy;
    wordCount = FIRST_LINE;
    
    for (currLine = file->head; currLine; currLine++)
    {
        if (currLine->label)
        {
            currSym->next = new_symbol(currLine->label,wordCount);
            currSym = currSym->next;
        }
        else if (currLine->kind == Request && currLine->statement.request.kind == EXTERN)
        {
            currSym->next = new_symbol(currLine->statement.request.data.str,0);
            currSym = currSym->next;
        }
        wordCount += increaseBy(currLine);
    }
    
    list->head = dummy->next;
    return list;
}

/* returns the number of lines the line will be in the object file */
int increaseBy (My_Line *line)
{
    //TODO: write the func
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
Symbol * new_symbol(char *symName, int symValue)
{
    /* TODO: write when it's time */
	Symbol *p;
	p = (Symbol *) malloc (sizeof(Symbol *));
    p->name = symName;
	p->value = symValue;
	p->next = NULL;
	return p;
}
