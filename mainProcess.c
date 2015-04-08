
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
        strcpy(end,".as");
        if (currAssemblyFile = fopen(argv[i], READ)) // what about other kinds of files
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
        undeclared = check_direct_address_method(file,symbols);
        if ((symbols = symbols_list(file)) && !undeclared)
        {
            makeObject(file,symbols);
            
            if (file->makeExt == MAKE)
                makeExtern(file,symbols);
            
            if (file->makeEnt == MAKE)
                makeEntry(file,symbols);
            
            free_list(symbols);
        }
        else
        {
            if (undeclared)
                printf("error: %d symbols were not declared",undeclared);
            else
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
int check_direct_address_method (My_File *file, SymbolList *list)
{
    My_Line *curr;
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
                if (!search_list(list,curr->statement.command.p1->value))
                    undeclared++;
                if (!search_list(list,second))
                    undeclared++;
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
                if (!search_list(list,curr->statement.command.p2->value))
                    undeclared++;
                if (!search_list(list,second))
                    undeclared++;
                *(second-1) = ',';
            }
        }
    }
    
    return undeclared;
}
