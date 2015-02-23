//
//  mainProcess.c
//  
//
//  Created by Guy on 2/19/15.
//
//

#include "mainProcess.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *currAssemblyFile;
    char *fileName = NULL;
    
    for (i = 1; i < argc; i++)
    {
        if (assemblyFile(argv[i],&fileName))
        {
            if (currAssemblyFile = fopen(argv[i],READ))
            {
                processFile(currAssemblyFile,fileName);
                fclose(currAssemblyFile);
            }
            free(fileName);
        }
    }
    return 0;
}

int assemblyFile(char *file, char **fileName)
{
    int length = 1;
    char *curr;
    curr = file;
    curr++;
    while (((*curr)!='\0')&&((*curr)!='.'))
    {
        length++;
        curr++;
    }
    if (!(strcmp(curr,ASSEMBLY)))
    {
        *fileName = (char *)malloc(length+1);
        strncpy(*fileName,file,length);
        return 1;
    }
    return 0;
}

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
    free_file(file);
}
