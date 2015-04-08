
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
long get_extra_words(parameter *p, SymbolList  *symbols, int curr_location)
{
	char *s = p->value;
	int n;
	Symbol sym;
	
	switch(p->kind)
	{
		case IMMEDIATE:
			s++;
			return atol(s);
			break;
		case DIRECT:
			sym = search_list(symbols, p->value);
			if(!sym)
				/* TODO: errorize */
				;
			return sym->value;
			break;
		case DISTANCE:
			{
				char *word = s;
				Symbol sym2;
				long res=0;
				long n=0;
				
				while (word && *word != ',')
					word++;
				if(!word)
					/* TODO: errorize */
					;
				*word = '\0';
				word++;
				if(!word)
					/* TODO: errorize */
					;
				sym = search_list(s);
				
				sym2 = search_list(word);
				if(!sym || !sym2)
					/* TODO: errorize */
					;
				
				#define abs (a) ((a) > 0 ? (a) : 0)
				res = abs(sym->value - curr_location);
				n  = abs(sym2->value - curr_location);
				if (res < n) res = n;
				n = abs(sym->value - sym2->value);
				if (res < n) res = n;
				return res;
			}
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
		
		switch(line->kind)
		{
			case Request:
				switch(line->statement.request.kind)
				{
					case STRING:
					case ENTRY:
					case EXTERN:
						fwrite(line->statement.request.data.nums.arr, 1, line->statement.request.data.nums.len, f);
						
						break;
					case DATA:
						fputs(f, line->statement.request.data.str);
				}
		}
		
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