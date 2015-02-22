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
#define OBJECT ".ob"
#define EXTERNALS ".ext"
#define ENTRIES ".ent"
#define READ "r" /* for fopen */
#define WRITE "w" /* for fopen */
#define A 0 /* absolute */
#define E 1 /* external */
#define R 2 /* relocatable */
enum {FIRST,SECOND,THIRD} /* group number */
enum {IMMEDIATE,DIRECT,DISTANCE,REGISTER} /* Addressing methods */

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
    int mode   :2; /* E/R/A */
    int target :2; /* target operand Addressing method*/
    int source :2; /* source operand Addressing method*/
    int opcode :4;
    int group  :2; /* the amount of operands for this instruction */
} instr_h; /* the instruction header - the first word of each instruction */

void process (FILE *file, char *fileName);
