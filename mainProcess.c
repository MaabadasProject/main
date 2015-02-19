//
//  mainProcess.c
//  
//
//  Created by Guy on 2/19/15.
//
//

#include "definitions.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *currAsFile, *currObFile, *currExtFile, *currEntFile;
    char *fileName;
    
    for (i = 1; i < argc; i++)
    {
        if (assemblyFile(argv[i],fileName) && (!(currAsFile = fopen(argv[i],READ))))
        {
            //TODO: write here process for assembly files
        }
    }
    return 0;
}

int assemblyFile(char *file, char *fileName)
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
    if ((*curr)=='.')
    {
        strncpy(fileName,file,length);
        return (!(strcmp(++curr,ASSEMBLY)));
    }
    return 0;
}
