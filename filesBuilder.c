
#include "filesBulilder.h"

int get_header(MY_Line *line)
{
    instr_h h;
    int amount_of_parameters;
    
    h.value = 0;
    amount_of_parameters = 0;
    
    h.bits.mode = A;
    if(line->statement.command->p1)
    {
        h.bits.target = line->statement.command->p1->kind;
        amount_of_parameters++;
    }
    if(line->statement.command->p2)
    {
        h.bits.source = line->statement.command->p2->kind;
        amount_of_parameters++;
    }
    h.bits.opcode = line->statement.command.opcode;
    h.bits.group = amount_of_parameters;
    
    return h.value;
}

/* returns the extra word that the addressing method needs. */
unsigned short get_parameter(parameter *p, SymbolList  *symbols, int position, unsigned int commandLineNum)
{
    unsigned short int value;
    char *second;
    Symbol *sym1,*sym2;
    
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
    }
    
    return value;
}

unsigned int abs (int n)
{
    if (n < 0)
        return -n;
    return n;
}

void writeObjLine(FILE *f, My_Line *line, SymbolList *symbols, unsigned int *lineNum)
{
    switch(line->kind)
    {
        case Request:
        {
            switch(line->statement.request.kind)
            {
                case ENTRY:
                case EXTERN:
                    break;
                case STRING:
                {
                    char *currChar = line->statement.request.data.str;
                    while (currChar)
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
        case Command:
        {
            fprintf(f, "%3X\t\t%.3X\n", *lineNum, get_header(line));
            (*lineNum)++;
            if (line->statement.command.p1)
            {
                unsigned short valP1, valP2;
                valP1 = get_parameter(line->statement.command.p1,symbols,FIRST,*lineNum);
                valP2 = get_parameter(line->statement.command.p2,symbols,SECOND,*lineNum);
                if (line->statement.command.p1->kind == REGISTER && line->statement.command.p2->kind == REGISTER)
                {
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, (valP1 + valP2));
                    (*lineNum)++;
                }
                else
                {
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP1);
                    (*lineNum)++;
                    
                    fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP2);
                    (*lineNum)++;
                }
                    
            }
            else if (line->statement.command.p2)
            {
                unsigned short valP2;
                valP2 = get_parameter(line->statement.command.p2,symbols,SECOND,*lineNum);
                fprintf(f,"%3X\t\t%.3X\n", *lineNum, valP2);
                (*lineNum)++;
            }
        }
    }
}

void writeExLine (FILE *f, My_Line *line, SymbolList *symbols, unsigned int *lineNum)
{
    if (line->kind == Command)
    {
        Symbol *sym;
        (*lineNum)++;
        
        if (line->statement.command.p1->kind == DIRECT)
        {
            sym = search_list(symbols,line->statement.command.p1->value);
            if (sym->value == 0)
                fprintf(f,"%s\t\t%3X\n",sym->name,*lineNum);
        }
        if (line->statement.command.p1->kind != NONE)
            (*lineNum)++;
        
        if (line->statement.command.p2->kind == DIRECT)
        {
            sym = search_list(symbols,line->statement.command.p2->value);
            if (sym->value == 0)
                fprintf(f,"%s\t\t%3X\n",sym->name,*lineNum);
        }
        if (line->statement.command.p2->kind != NONE)
            (*lineNum)++;
        
        if (line->statement.command.p1->kind == REGISTER && line->statement.command.p2->kind == REGISTER)
            (*lineNum)--;
    }
    else
    {
        if (line->statement.Request.kind == DATA)
            (*lineNum) += line->statement.Request.data.nums.len;
        else if (line->statement.Request.kind == STRING)
            (*lineNum) += strlen(line->statement.Request.data.str)+1;
    }
}

void writeEnLine (FILE *f, My_Line *line, SymbolList *symbols)
{
    if (line->kind == Request && line->statement.reques.kind == ENTRY)
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

void makeObject(MY_FILE *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char obName[strlen(filename)+strlen(OBJECT)];
    unsigned int *lineNum,lineNumber;
    
    strcpy(obName,filename);
    strcat(obName,OBJECT);
    lineNumber = FIRST_LINE;
    lineNum = &lineNumber;
    
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
            writeObjLine(f, curr, symbols, lineNum);
            curr++;
        }
        
        fclose(f);
    }
}

void makeExtern(MY_FILE *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char exName[strlen(filename)+strlen(EXTERNALS)];
    unsigned int *lineNum,lineNumber;
    
    strcpy(exName,filename);
    strcat(exName,EXTERNALS);
    lineNumber = FIRST_LINE;
    lineNum = &lineNumber;
    
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
            writeExLine(f, curr, symbols, lineNum);
            curr++;
        }
        
        fclose(f);
    }
}

void makeEntry(MY_FILE *my_f, SymbolList *symbols, char *filename)
{
    FILE *f;
    char enName[strlen(filename)+strlen(ENTRIES)];
    
    strcpy(enName,filename);
    strcat(enName,ENTRIES);
    
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
            curr++;
        }
        
        fclose(f);
    }
}
