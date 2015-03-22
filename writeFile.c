#include "definitions.h"
#include <stdio>

/* this file should be merged with my_file */

instr_h_b *get_header(Line *line)
{
	instr_h_b *h;
	int amount_of_parameters = 0;
	
	h = (*instr_h_b) malloc(sizeof(instr_h_b);
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

void write_header(FILE *f, Line *line)
{
	instr_h_b *h;
	h = get_header(line);
	fwrite(h, 2 /* maybe? */, 1, f);
	free(instr_h_b);
}

	