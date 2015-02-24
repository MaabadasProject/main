//
//  fileProcess.h
//  
//
//  Created by Guy on 2/22/15.
//
//

#include "definitions.h"

int get_opcode(void);

SymbolList symbols_list (FILE *);

SymbolList * new_symbolList();

Symbol * new_symbol(char *, long);

void free_list (SymbolList);

Symbol * get_symbol_name (FILE *, long, SymbolList);
