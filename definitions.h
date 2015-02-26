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
enum Addressing {IMMEDIATE,DIRECT,DISTANCE,REGISTER} /* Addressing methods */
#define MAX_SYMBOL_NAME 30

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

#ifndef registers
char *registers[8] =
    {"r0"
    ,"r1"
    ,"r2"
    ,"r3"
    ,"r4"
    ,"r5"
    ,"r6"
    ,"r7"};
#endif

typedef struct line
{
    char *label;
	enum {Command, Request, Error} kind; /* the kind of the assembly statement */
	union /* the data of this assembly statement */
    {
		struct /* a request */
        {
			enum {DATA, STRING, ENTRY, EXTERN} kind;
			union
            {
                int *arr; /* for .data */
                char *str; /* for .string, .entry and .extern */
            } data; /* the data of the command; unimplemented */
		} request;
		struct /* a command */
        {
			int opcode; /* the location in the opcodes table */
			parameter *p1, *p2; /* the two parameters. consider changing the type (probably a location in the symbol table?) */
		} command;
	} statement;
    struct line *next;
} My_Line;

typedef struct
{
    enum Addressing kind;
    char *value;
}parameter;

typedef struct
{
    My_Line *firstLine;
    enum {NO,YES} makeOb;  /* if syntax error was found */
    enum {NO,YES} makeExt; /* if .extern was found */
    enum {NO,YES} maxeEnt; /* if .entry was found */
}My_File;

typedef struct {
    int mode   :2; /* E/R/A */
    int target :2; /* target operand Addressing method*/
    int source :2; /* source operand Addressing method*/
    int opcode :4;
    int group  :2; /* the amount of operands for this instruction */
} instr_h; /* the instruction header - the first word of each instruction */

typedef struct Node
{
    char *name;
    long value;
    struct Node *next;
} Symbol;

typedef struct
{
    Symbol *head;
} SymbolList;
