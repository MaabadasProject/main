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
    FILE *currAsFile, *currObFile, *currExtFile, *currEntFile;
    char *fileName = NULL;
    
    for (i = 1; i < argc; i++)
    {
        if ((currAsFile = fopen(argv[i],READ)) && (assemblyFile(argv[i],&fileName)))
        {
            //  TODO: write here process for assembly files
            free(fileName);
        }
        fclose(currAsFile);
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
