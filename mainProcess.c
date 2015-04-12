
/* this file contains the main function */
/* errors messeges will be printed to stderr */

#include "mainProcess.h"

/* four global arrays for testing the correction of the input */
char *opcodes[NUMBER_OF_COMMANDS] =
    { "mov"
    , "cmp"
    , "add"
    , "sub"
    , "not"
    , "clr"
    , "lea"
    , "inc"
    , "dec"
    , "jmp"
    , "bne"
    , "red"
    , "prn"
    , "jsr"
    , "rts"
    , "stop"};

int legal_adressing_methods[NUMBER_OF_COMMANDS][2][NUMBER_OF_ADDRESSING_METHODS] =
    /*source  ||  destination*/
    {{{1,1,1,1},	{0,1,0,1}},
    {{1,1,1,1},		{1,1,1,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{0,0,0,0},		{0,0,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,1,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{1,1,1,1}},
    {{0,0,0,0},		{0,1,0,0}},
    {{0,0,0,0},		{0,0,0,0}},
    {{0,0,0,0},		{0,0,0,0}}};

char *requests[NUMBER_OF_REQUESTS] =
    { ".data"
    , ".string"
    , ".entry"
    , ".extern"};

char *registers[NUMBER_OF_REGISTERS] =
    { "r0"
    , "r1"
    , "r2"
    , "r3"
    , "r4"
    , "r5"
    , "r6"
    , "r7"};

/* the main main function. */
/*opens files and process them with processFile */
int main(int argc, char *argv[])
{
    int i;
    FILE *currAssemblyFile;
    char *fileName;
    
    for (i = 1; i < argc; i++)
    {
        if ((fileName = assemblyFile(argv[i])))
        {
            if ((currAssemblyFile = fopen(argv[i], "r")))
            {
                processFile(currAssemblyFile, fileName);
                fclose(currAssemblyFile);
            }
            else
            {
                fprintf(stderr,"error: the file %s could not be found.\n",argv[i]);
            }
            free(fileName);
        }
        else
        {
            fprintf(stderr,"error: the file %s is not an assembly file.\n",argv[i]);
        }
    }
    return 0;
}

/* returns the name of the file without the ending if it is an .as file */
/* returns NULL if if it is not an .as file */
char * assemblyFile(char *file)
{
    int length;
    char *curr, *name;
    
    curr = file;
    length = 0;
    
    while (((*curr)!='\0')&&((*curr)!='.'))
    {
        length++;
        curr++;
    }
    if (!(strcmp(curr,ASSEMBLY)))
    {
        name = (char *)malloc(length+1);
        
        if (name == NULL)
        {
            fprintf(stderr, "error: out of memory\n");
            exit(1);
        }
        
        strncpy(name,file,length);
        name[length] = '\0';
        return name;
    }
    return NULL;
}

/* this is the whole process for a file. */
/* this will get an assembly file (the first argument) ready for reading, and the files name. */
void processFile (FILE *asFile, char *fileName)
{
    SymbolList *symbols;
    My_File *file;
    int undeclared = 0;
    
    file = new_file(asFile);
    
    if (file->makeOb == MAKE)
    {
        symbols = symbols_list(file);
        if (symbols)
        {
            undeclared = check_direct_variables(file,symbols);
            if (undeclared == -1)
            {
                fprintf(stderr,"error: direct addressing method parameters must not be external.\n");
            }
            else if (!undeclared)
            {
                makeObject(file, symbols, fileName);
                
                if (file->makeExt == MAKE)
                    makeExtern(file, symbols, fileName);
                
                if (file->makeEnt == MAKE)
                    makeEntry(file, symbols, fileName);
            }
            else
            {
                if (undeclared == 1)
                    fprintf(stderr,"error: one symbol was not declared.\n");
                else
                    fprintf(stderr,"error: %d symbols were not declared.\n",undeclared);
            }
            free_list(symbols);
        }
        else
        {
            fprintf(stderr,"error: label can only be defined once.\n");
        }
    }
    else
    {
        printErrors(file);
    }
    free_file(file);
}

/* checks if all the direct variables were declared */
/* returns the number of undeclared parameters, or -1 if external variable was used as a distance parameter */
int check_direct_variables (My_File *file, SymbolList *list)
{
    My_Line *curr;
    Symbol *sym;
    int undeclared;
    char *second;
    
    undeclared = 0;
    curr = file->firstLine;
    
    while (curr)
    {
        if (curr->kind == COMMAND)
        {
            if (curr->statement.command.p1->kind == DIRECT)
            {
                if (!search_list(list,curr->statement.command.p1->value))
                    undeclared++;
            }
            else if (curr->statement.command.p1->kind == DISTANCE)
            {
                second = curr->statement.command.p1->value;
                while ((*second) != ',')
                    second++;
                *second = '\0';
                second++;
                
                sym = search_list(list,curr->statement.command.p1->value);
                if (!sym)
                {
                    undeclared++;
                }
                else if (sym->value == 0)
                {
                    return -1;
                }
                
                sym = search_list(list,second);
                if (!sym)
                {
                    undeclared++;
                }
                else if (sym->value == 0)
                {
                    return -1;
                }
                
                *(second-1) = ',';
            }
            
            if (curr->statement.command.p2->kind == DIRECT)
            {
                if (!search_list(list,curr->statement.command.p2->value))
                    undeclared++;
            }
            else if (curr->statement.command.p2->kind == DISTANCE)
            {
                second = curr->statement.command.p2->value;
                while ((*second) != ',')
                    second++;
                *second = '\0';
                second++;
                
                sym = search_list(list,curr->statement.command.p2->value);
                if (!sym)
                {
                    undeclared++;
                }
                else if (sym->value == 0)
                {
                    return -1;
                }
                
                sym = search_list(list,second);
                if (!sym)
                {
                    undeclared++;
                }
                else if (sym->value == 0)
                {
                    return -1;
                }
                
                *(second-1) = ',';
            }
        }
        curr = curr->next;
    }
    
    return undeclared;
}

/* if the file contains errors, this function will be called. */
/* prints all the errors in the file to stderr */
void printErrors(My_File *file)
{
    My_Line *curr;
    int lineNum;
    
    curr = file->firstLine;
    lineNum = 1;
    
    while (curr)
    {
        if (curr->kind == ERROR)
        {
            fprintf(stderr,"error: line %d - %s.\n", lineNum, curr->statement.error);
        }
        curr = curr->next;
        lineNum++;
    }
}
