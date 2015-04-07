
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
            printf("error: the file %s coud not be found\n",fileName);
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
    
    file = new_file(asFile);
    
    if (file->makeOb == MAKE)
    {
        if ((symbols = symbols_list(file)))
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
            printf("error: label can only be defined once.\n");
        }
    }
    else
    {
        printErrors(file);
    }
    free_file(file);
}
