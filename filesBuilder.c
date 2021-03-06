
/* this file creates the output files */

#include "filesBuilder.h"

/* makes the .ob file out of the My_File list */
void makeObject(My_File *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char obName[strlen(filename)+strlen(OBJECT)+1];
    unsigned int lineNumber;
    
    strcpy(obName,filename);
    strcat(obName,OBJECT);
    obName[strlen(filename)+strlen(OBJECT)] = '\0';
    lineNumber = FIRST_LINE;
    
    f = fopen(obName, "w");
    if (!f)
    {
        fprintf(stderr,"error: the file %s cannot be written to.\n", obName);
    }
    else
    {
        My_Line *curr;
        
        curr = my_f->firstLine;
        while (curr)
        {
            writeObjLine(f, curr, symbols, &lineNumber);
            curr = curr->next;
        }
        
        fclose(f);
    }
}

/* makes the .ext file out of the My_File list */
void makeExtern(My_File *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char exName[strlen(filename)+strlen(EXTERNALS)+1];
    unsigned int lineNumber;
    
    strcpy(exName,filename);
    strcat(exName,EXTERNALS);
    exName[strlen(filename)+strlen(EXTERNALS)] = '\0';
    lineNumber = FIRST_LINE;
    
    f = fopen(exName, "w");
    if (!f)
    {
        fprintf(stderr,"error: the file %s cannot be written to.\n", exName);
    }
    else
    {
        My_Line *curr;
        
        curr = my_f->firstLine;
        while (curr)
        {
            writeExLine(f, curr, symbols, &lineNumber);
            curr = curr->next;
        }
        
        fclose(f);
    }
}

/* makes the .ent file out of the My_File list */
void makeEntry(My_File *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char enName[strlen(filename)+strlen(ENTRIES)+1];
    
    strcpy(enName,filename);
    strcat(enName,ENTRIES);
    enName[strlen(filename)+strlen(ENTRIES)] = '\0';
    
    f = fopen(enName, "w");
    if (!f)
    {
        fprintf(stderr,"error: the file %s cannot be written to.\n", enName);
    }
    else
    {
        My_Line *curr;
        
        curr = my_f->firstLine;
        while (curr)
        {
            writeEnLine(f, curr, symbols);
            curr = curr->next;
        }
        
        fclose(f);
    }
}

/* return the value of the command header */
int get_header(My_Line *line)
{
    instr_h h;
    int amount_of_parameters;
    
    h.value = 0;
    amount_of_parameters = 0;
    
    h.bits.mode = A;
    if(line->statement.command.p1->kind != EMPTY)
    {
        h.bits.source = line->statement.command.p1->kind;
        amount_of_parameters++;
    }
    if(line->statement.command.p2->kind != EMPTY)
    {
        h.bits.target = line->statement.command.p2->kind;
        amount_of_parameters++;
    }
    h.bits.opcode = line->statement.command.opcode;
    h.bits.group = amount_of_parameters;
    
    return h.value;
}

/* returns the value of the parameter it gets. */
/* position should be 0 for first and 1 for second */
/* commandLineNum if the line number of the command */
unsigned short get_parameter(parameter *p, SymbolList  *symbols, int position, unsigned int commandLineNum)
{
    unsigned short int value;
    char *second;
    Symbol *sym1,*sym2;
    
    value = 0;
    switch (p->kind) {
        case IMMEDIATE:
            value = atoi(p->value);
            value <<= 2;
            value += A;
            value %= MAX_NUM;
            break;
        case DIRECT:
            sym1 = search_list(symbols,p->value);
            value = sym1->value;
            value <<= 2;
            if (value == 0)
                value += E;
            else
                value += R;
            value %= MAX_NUM;
            break;
        case DISTANCE:
            second = p->value;
            while ((*second) != ',')
                second++;
            second++;
            (*(second-1)) = '\0';
            
            sym1 = search_list(symbols,p->value);
            sym2 = search_list(symbols,second);
            value = abs(sym1->value - sym2->value);
            if (value < abs(sym1->value - commandLineNum))
                value = abs(sym1->value - commandLineNum);
            if (value < abs(sym2->value - commandLineNum))
                value = abs(sym2->value - commandLineNum);
            
            (*(second-1)) = ',';
            
            value <<= 2;
            value += A;
            value %= MAX_NUM;
            break;
        case REGISTER:
            value = ((p->value)[1] - '0');
            value <<= 2;
            if (position == FIRST)
                value <<= 5;
            value += A;
            break;
        case NONE:
        case EMPTY:
            break;
    }
    
    return value;
}

/* prints in f the curr line value. f is an .ob file */
void writeObjLine(FILE *f, My_Line *line, SymbolList *symbols, unsigned int *lineNum)
{
    switch(line->kind)
    {
        case REQUEST:
        {
            switch(line->statement.request.kind)
            {
                case ENTRY:
                case EXTERN:
                    break;
                case STRING:
                {
                    char *currChar = line->statement.request.data.str;
                    while (*currChar)
                    {
                        fprintf(f,"%3X\t\t%.3X\n", *lineNum, (unsigned int)(*currChar));
                        currChar++;
                        (*lineNum)++;
                    }
                    fprintf(f,"%3X\t\t000\n", *lineNum);
                    (*lineNum)++;
                }
                    break;
                case DATA:
                {
                    int i;
                    for (i = 0; i < line->statement.request.data.nums.len; i++, (*lineNum)++)
                        fprintf(f,"%3X\t\t%.3X\n", *lineNum, ((unsigned)line->statement.request.data.nums.arr[i] % MAX_NUM));
                }
                    break;
            }
        }
            break;
        case COMMAND:
        {
            fprintf(f, "%3X\t\t%.3X\n", *lineNum, get_header(line));
            if (line->statement.command.p1->kind != EMPTY)
            {
                unsigned short valP1, valP2;
                valP1 = get_parameter(line->statement.command.p1,symbols,FIRST,*lineNum);
                valP2 = get_parameter(line->statement.command.p2,symbols,SECOND,*lineNum);
                (*lineNum)++;
                if (line->statement.command.p1->kind == REGISTER && line->statement.command.p2->kind == REGISTER)
                {
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, (valP1 + valP2));
                }
                else
                {
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP1);
                    (*lineNum)++;
                    
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP2);
                }
                    
            }
            else if (line->statement.command.p2->kind != EMPTY)
            {
                unsigned short valP2;
                valP2 = get_parameter(line->statement.command.p2,symbols,SECOND,*lineNum);
                (*lineNum)++;
                fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP2);
            }
            (*lineNum)++;
        }
        case ERROR:
            break;
    }
}

/* if curr line (line) contains external variable, */
/* prints in f the name of the variable and the line number (lineNum) */
/* f is an .ext file */
void writeExLine (FILE *f, My_Line *line, SymbolList *symbols, unsigned int *lineNum)
{
    if (line->kind == COMMAND)
    {
        Symbol *sym;
        (*lineNum)++;
        
        if (line->statement.command.p1->kind == DIRECT)
        {
            sym = search_list(symbols,line->statement.command.p1->value);
            if (sym->value == 0)
                fprintf(f,"%s\t\t%3X\n",sym->name,*lineNum);
        }
        if (line->statement.command.p1->kind != EMPTY)
            (*lineNum)++;
        
        if (line->statement.command.p2->kind == DIRECT)
        {
            sym = search_list(symbols,line->statement.command.p2->value);
            if (sym->value == 0)
                fprintf(f,"%s\t\t%3X\n",sym->name,*lineNum);
        }
        if (line->statement.command.p2->kind != EMPTY)
            (*lineNum)++;
        
        if (line->statement.command.p1->kind == REGISTER && line->statement.command.p2->kind == REGISTER)
            (*lineNum)--;
    }
    else
    {
        if (line->statement.request.kind == DATA)
            (*lineNum) += line->statement.request.data.nums.len;
        else if (line->statement.request.kind == STRING)
            (*lineNum) += strlen(line->statement.request.data.str)+1;
    }
}

/* if the curr line is a request line from type entry, */
/* prints the name of the variable and its value in f */
/* f is an .ent file */
void writeEnLine (FILE *f, My_Line *line, SymbolList *symbols)
{
    if (line->kind == REQUEST && line->statement.request.kind == ENTRY)
    {
        Symbol *sym = search_list(symbols,line->statement.request.data.str);
        if (sym)
        {
            fprintf(f,"%s\t\t%3X\n",sym->name,sym->value);
        }
        else
        {
            fprintf(stderr,"error: variable %s wasn't declared.\n",line->statement.request.data.str);
        }
    }
}
