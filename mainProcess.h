//
//  mainProcess.h
//  
//
//  Created by Guy on 2/20/15.
//
//

#include "definitions.h"

/* if file is an assembly file, puts its name in name and returns 1. else return 0. */
int assemblyFile(char *file,char **name);

void processFile (FILE *, char *);

My_File new_file(FILE *);

SymbolList symbols_list(FILE *);

void free_list(SymbolList);

void free_file(My_File);
