
#include "fileProcess.h"

/* this is the whole process. */
/* this will get an assembly file (the first argument) ready for reading, and the files name. */
void processFile (FILE *asFile, char *fileName)
{
    SymbolList symbols;
    My_File file;
    
    file = new_file(asFile);
    symbols = symbols_list(asFile);
    
    /* TOBE: continued... */
    
    free_list(symbols);
}


/* not working */
Instr *parse() {
    Instr *instr;
    char c;
    char buff[31];
    int i = 0; /* the index to the label buffer */
    instr = (Instr *) malloc(sizeof(Instr));
    
    c = getc();
    if (c != ' ')
    {
        for(;;) {
            c = getc();
            if (c != ' ' && c != ':')
                buff[i++] = c;
            else
                break;
        }
        if (c == ':')
            instr->label = "" /* TODO instr->label = copy_of(buff) */
            if (c == ' ')
            /* TODO: DO STUFF */
                ungetc(c);
        getc[i++] = '\0'; /* null terminate the buffer */
    }
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
SymbolList symbols_list (FILE *asFile)
{
    SymbolList list;
    Symbol *currSymbol, *lastSymbol;
    fpos_t *startPos;
    
    &list = new_symbolList();
    fseek(asFile,0L,SEEK_SET);
    
    list.head = get_symbol_name(asFile,list);
    lastSymbol = list.head;
    if (!lastSymbol)
    {
        while ((currSymbol = get_symbol_name(asFile,list)))
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
Symbol * get_symbol_name (FILE *asFile, SymbolList knoneSymbols)
{
    static long lineCount = 0;
    char name[MAX_SYMBOL_NAME + 2]; /* +2 for ':' and '\0' */
    Symbol *sym;
    
    while (fscanf(asFile, "%.*s", MAX_SYMBOL_NAME+1, name) == 1)
    {
        if (isSymbol(name))
        {
            sym = new_symbol(name,lineCount);
            set_new_line();
            lineCount++;
            return sym;
        }
        else
        {
            set_new_line();
            lineCount++;
        }
    }
    return NULL;
}
