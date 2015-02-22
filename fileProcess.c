//
//  fileProcess.c
//  
//
//  Created by Guy on 2/22/15.
//
//

#include "fileProcess.h"

/* this is the whole process. */
/* this will get an assembly file (the first argument) ready for reading, and the files name. */
void processFile (FILE *asFile, char *fileName)
{
    
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
