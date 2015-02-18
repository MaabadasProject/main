//
//  definitions.c
//  
//
//  Created by Guy on 2/18/15.
//
//

#include <stdio.h>

#define A 0 /* absolute */
#define E 1 /* external */
#define R 2 /* relocatable */

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

typedef struct {
    int group :2; /* the amount of operands for this instruction */
    int opcode:4;
    int source:2; /* source operand */
    int target:2; /* target operand */
    int mode  :2; /* E/R/A */
} instr_h; /* the instruction header - the first word of each instruction */
