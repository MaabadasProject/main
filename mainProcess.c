
#include "mainProcess.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *currAssemblyFile;
    
    for (i = 1; i < argc; i++)
    {
        char fileName[strlen(argv[i]) + 4], *end;
        strcpy(fileName,argv[i]);
        end = fileName + strlen(argv[i]);
        strcpy(end,ASSEMBLY);
        if (currAssemblyFile = fopen(argv[i], "r"))
        {
            processFile(currAssemblyFile, argv[i]);
            fclose(currAssemblyFile);
        }
        else
        {
            printf("error: the file %s coud not be found.\n",fileName);
        }
    }
    return 0;
}

/* this is the whole process for a file. */
/* this will get an assembly file (the first argument) ready for reading, and the files name. */
void processFile (FILE *asFile, char *fileName)
{
    SymbolList *symbols;
    My_File *file;
    int undeclared;
    
    file = new_file(asFile);
    undeclared = 0;
    
    if (file->makeOb == MAKE)
    {
        if ((symbols = symbols_list(file)))
        {
            undeclared = check_direct_variables(file,symbols);
            if (undeclared == -1)
            {
                printf("error: direct addressing method parameters must not be external.");
            }
            else if (!undeclared)
            {
                makeObject(file,symbols,fileName);
                
                if (file->makeExt == MAKE)
                    makeExtern(file,symbols,fileName);
                
                if (file->makeEnt == MAKE)
                    makeEntry(file,symbols,fileName);
            }
            else
            {
                printf("error: %d symbols were not declared",undeclared);
            }
            free_list(symbols);
        }
        else
        {
            printf("error: label can only be defined once.\n");
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
    curr = file->FirstLine;
    
    while (curr)
    {
        if (curr->kind == Command)
        {
            if (curr->statement.command.p1->kind == DIRECT)
                if (!search_list(list,curr->statement.command.p1->value))
                    undeclared++;
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
                if (!search_list(list,curr->statement.command.p2->value))
                    undeclared++;
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
        curr++;
    }
    
    return undeclared;
}

void printErrors(My_File *file)
{
    My_Line *curr;
    
    curr = file->firstLine;
    
    while (curr)
    {
        if (curr->kind == Error)
        {
            printf("error: %s\n",curr->statement.error);
        }
        curr = curr->next;
    }
}
