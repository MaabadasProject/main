
/* this file contains functions for making a symbols list,
    and also to search a smbol in it */

#include "Symbols.h"

/* returns a SymbolList contains the symbols in the file */
SymbolList * symbols_list(My_File *file)
{
    SymbolList *list;
    Symbol *currSym, *dummy;
    My_Line *currLine;
    int wordCount;
    
    list = new_symbolList();
    dummy = new_symbol("",0);
    list->head = dummy;
    currSym = dummy;
    wordCount = FIRST_LINE;
    
    for (currLine = file->firstLine; currLine; currLine = currLine->next)
    {
        if (currLine->label[0])
        {
            if (!search_list(list,currLine->label))
            {
                currSym->next = new_symbol(currLine->label,wordCount);
                currSym = currSym->next;
            }
            else
            {
                free_list(list);
                return NULL;
            }
        }
        else if (currLine->kind == REQUEST && currLine->statement.request.kind == EXTERN)
        {
            if (!search_list(list,currLine->statement.request.data.str))
            {
                currSym->next = new_symbol(currLine->statement.request.data.str,0);
                currSym = currSym->next;
            }
            else
            {
                list->head = dummy;
                free_list(list);
                return NULL;
            }
        }
        wordCount += increaseBy(currLine);
    }
    
    list->head = list->head->next;
    free(dummy);
    return list;
}

/* returns the number of lines the line will be in the object file */
int increaseBy (My_Line *line)
{
    if (line->kind == COMMAND)
    {
        if (line->statement.command.p1->kind == REGISTER && line->statement.command.p2->kind == REGISTER)
            return 2;
        if (line->statement.command.p1->kind != EMPTY)
            return 3;
        if (line->statement.command.p2->kind != EMPTY)
            return 2;
        return 1;
    }
    else /* if line->kind == REQUEST */
    {
        if (line->statement.request.kind == DATA)
            return line->statement.request.data.nums.len;
        else if (line->statement.request.kind == STRING)
            return (int)strlen(line->statement.request.data.str) + 1;
        else
            return 0;
    }
}

/* frees the symbols list */
void free_list (SymbolList *list)
{
    if (list->head)
    {
        Symbol *curr, *post;
        curr = list->head;
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
    
    if (list == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    list->head = NULL;
    return list;
}

/* returns a pointer to a new Symbol created using malloc */
Symbol * new_symbol(char *symName, int symValue)
{
	Symbol *p;
    p = (Symbol *) malloc (sizeof(Symbol));
    
    if (p == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    p->name = symName;
	p->value = symValue;
	p->next = NULL;
	return p;
}

/* search the symbol named "name" in SymbolList "list"
   returns pointer to the symbol if it was found.
   returns NULL if not. */
Symbol *search_list(SymbolList *list, char *name)
{
	Symbol *curr = list->head;
	while (curr)
	{
		if(!strcmp(curr->name, name))
			return curr;
		curr = curr->next;
	}
	return NULL;
}
