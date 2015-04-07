
#include "filesBulilder.h"




instr_h_b *get_header(MY_Line *line)
{
    instr_h_b *h;
    int amount_of_parameters = 0;
    
    h = (*instr_h_b) malloc(sizeof(instr_h_b));
    switch(line->kind)
    {
        case Request:
        case Error:
            h->mode=0; break;
        case Command:
        /* first figure out the A/R/E */
            if(line->statement.command->p1)
            {
                h.target = line->statement.command->p1->kind;
                amount_of_parameters++;
            }
            if(line->statement.command->p2)
            {
                h.source = line->statement.command->p2->kind;
                amount_of_parameters++;
            }
            h.opcode = line->statement.command.opcode;
            h.group = amount_of_parameters;
    }
    
    return istr_h_b;
}

/* returns the extra word that the addressing method needs. */
long get_extra_words(parameter *p, SymbolList  *symbols)
{
	char *s = p->value;
	int n;
	Symbol s;
	
	switch(p->kind)
	{
		case IMMEDIATE:
			s++;
			return atol(s);
			break;
		case DIRECT:
			s = search_list(symbols, p->value);
			if(!s)
				/* TODO: errorize */
				;
			return s->value;
			break;
		case DISTANCE:
			/* TODO */
			break;
		case REGISTER:
			s++;
			n = atoi(s);
			if (n < 0 || n > 7)
				/* TODO: errorize */
				;
			return n;
	}
	
}


void write_line(FILE *f, MY_Line *line)
{
    instr_h_b *h;
    
    for( ; line ; line = line->next)
    {
        h = get_header(line);
        fwrite(h, 2 /* maybe? */, 1, f);
        free(instr_h_b);
    }
}

/* this functions needs better care */
void write_file(FILE *f, MY_FILE *my_f)
{
    wrie_line(f, my_f->firstLine)
}

void handle_file(char *filename, MY_FILE *my_f)
{
    FILE *f;
    f = fopen(filename, "w");
    if (!f)
    {
        fprintf(stderr, "the file %s cannot be written to", filename);
        exit(1);
    }
    write_file(f, my_f);
}