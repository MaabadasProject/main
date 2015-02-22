//
//  definitions.h
//
//
//  Created by Guy on 2/19/15.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSEMBLY ".as"
#define READ "r" /* for fopen */
#define WRITE "w" /* for fopen */
#define A 0 /* absolute */
#define E 1 /* external */
#define R 2 /* relocatable */

#ifndef opcodes
char *opcodes[16] =
    {"mov"
    ,"cmp"
    ,"add"
    ,"sub"
    ,"not"
    ,"clr"
    ,"lea"
    ,"inc"
    ,"dec"
    ,"jmp"
    ,"bne"
    ,"red"
    ,"prn"
    ,"jsr"
    ,"rts"
    ,"stop"};
#endif

typedef struct {
    int group :2; /* the amount of operands for this instruction */
    int opcode:4;
    int source:2; /* source operand */
    int target:2; /* target operand */
    int mode  :2; /* E/R/A */
} instr_h; /* the instruction header - the first word of each instruction */

void process (FILE *file, char *fileName);
