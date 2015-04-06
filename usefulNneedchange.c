
#include "fileProcess.h"


/* not working */
Instr *parse() {
    Instr *instr;
    char c;
    char buff[31];
    int i = 0; /* the index to the label buffer */
    instr = (Instr *) malloc(sizeof(Instr));
    
    c = getc();
    if (c != ' ')
    {
        for(;;) {
            c = getc();
            if (c != ' ' && c != ':')
                buff[i++] = c;
            else
                break;
        }
        if (c == ':')
            instr->label = "" /* TODO instr->label = copy_of(buff) */
            if (c == ' ')
            /* TODO: DO STUFF */
                ungetc(c);
        getc[i++] = '\0'; /* null terminate the buffer */
    }
}

/* this will parse an opcode and return it's position in the table opcodes. */
/* if an opcode cannot be parsed it will return -1 */
int get_opcode()
{
    int i;
    for(i = 0;i < 16;i++)
    {
        int j;
        for (j = 0;opcodes[i][j];j++)
        {
            int c = getc();
            if (c != opcodes[i][j])
            {
                ungetc(c);
                for (j-- ; j >= 0; j--)
                    ungetc(opcodes[i][j]);
                break;
            }
        }
        if (j != -1) /* if parsing succeeded */
            return i;
    }
    puts("unrecognized opcode");
    return -1;
}

