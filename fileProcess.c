
#include "fileProcess.h"

/* this is the whole process. */
/* this will get an assembly file (the first argument) ready for reading, and the files name. */
void processFile (FILE *asFile, char *fileName)
{
    SymbolList symbols;
    
    symbols = symbol_list(asFile);
    
    /* TOBE: continue... */
    
    free_list(symbols);
}

/* this will parse an opcode and return it's position in the table opcodes. */
/* if an opcode cannot be parsed it will return -1 */
int get_opcode()
{
  int i;
  for(i = 0;i < 16;i++)
  {
    int j;
    for (j = 0;opcodes[i][j];j++)
    {
      int c = getc();
      if (c != opcodes[i][j])
      {
		ungetc(c);
		for (j-- ; j >= 0; j--)
		  ungetc(opcodes[i][j]);
		break;
      }
    }
    if (j != -1) /* if parsing succeeded */
      return i;
  }
  puts("unrecognized opcode");
  return -1;
}

/* this will return a SymbolList contains all the symbols in the assembly file */
SymbolList symbol_list (FILE *asFile)
{
    SymbolList list;
    Symbol *currSymbol, *lastSymbol;
    
    &list = new_symbolList();
    fseek(asFile,0L,SEEK_SET);
    
    list.head = get_symbol_name(asFile,ftell(asFile),list); /* TODO: write this function */
    lastSymbol = list.head;
    if (!lastSymbol)
    {
        while ((currSymbol = get_symbol_name(asFile,ftell(asFile),list))) /* TODO: write this function */
        {
            lastSymbol->next = currSymbol;
            lastSymbol = lastSymbol->next;
        }
    }
    return list;
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
Symbol * new_symbol(char *symName, unsigned long int symAddress)
{
    Symbol *sym;
    sym = (Symbol *)malloc(sizeof(Symbol));
    strcpy(sym->name,symName);
    sym->address = symAddress;
    sym->next = NULL;
    return sym;
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

/* this will return each time a new symbol */
/* returns NULL if there are no more symbols in the file */
Symbol * get_symbol_name (FILE *asFile, long startPos, SymbolList knoneSymbols)
{
    /*TODO: write this function */
}
