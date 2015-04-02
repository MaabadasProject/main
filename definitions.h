
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASSEMBLY ".as"
#define OBJECT ".ob"
#define EXTERNALS ".ext"
#define ENTRIES ".ent"
#define READ "r" /* for fopen */
#define WRITE "w" /* for fopen */
#define A 0 /* absolute */
#define E 1 /* external */
#define R 2 /* relocatable */
#define MAX_SYMBOL_NAME 30
#define MAX_WORD_LENGTH 50
#define MAX_ERROR_LENGTH 50
#define MAX_LINE_LENGTH 80
#define MAX_REG_LEN 2
#define MAX_COM_LEN 4
#define MAX_REQ_LEN 7
#define NUMBER_OF_COMMANDS 16
#define NUMBER_OF_REGISTERS 8
#define NUMBER_OF_REQUESTS 4
#define NUMBER_OF_ADDRESSING_METHODS 4
#define FIRST_LINE 100
#define DELIMITERS " \t,:"
enum {FIRST, SECOND, THIRD} /* group number */
enum Addressing {IMMEDIATE, DIRECT, DISTANCE, REGISTER} /* Addressing methods */

#ifndef opcodes
char *opcodes[NUMBER_OF_COMMANDS] =
    { "mov"
    , "cmp"
    , "add"
    , "sub"
    , "not"
    , "clr"
    , "lea"
    , "inc"
    , "dec"
    , "jmp"
    , "bne"
    , "red"
    , "prn"
    , "jsr"
    , "rts"
    , "stop"};
#endif

#ifndef legal_adressing_methods
int legal_adressing_methods[NUMBER_OF_COMMANDS][2][NUMBER_OF_ADDRESSING_METHODS] =
	/* source  ||  destination */
    {{{1,1,1,1},	{0,1,0,1}},
    {{1,1,1,1},		{1,1,1,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,1,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,0,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{0,1,1,1}},
    {{0,0,0,0},		{1,1,1,1}},
    {{0,0,0,0},		{0,1,0,0}},
    {{0,0,0,0},		{0,0,0,0}},
    {{0,0,0,0},		{0,0,0,0}}}
#endif
	 

#ifndef opcodes
char *requests[NUMBER_OF_REQUESTS] =
    { ".data"
    , ".string"
    , ".entry"
    , ".extern"};
#endif

#ifndef registers
char *registers[NUMBER_OF_REGISTERS] =
    { "r0"
    , "r1"
    , "r2"
    , "r3"
    , "r4"
    , "r5"
    , "r6"
    , "r7"};
#endif

typedef struct line
{
    char label[MAX_SYMBOL_NAME];
	enum {Command, Request, Error} kind; /* the kind of the assembly statement */
	union /* the data of this assembly statement */
    {
		struct /* a request */
        {
			enum {DATA, STRING, ENTRY, EXTERN} kind;
			union
            {
                struct
                {
                    int *arr; /* for .data */
                    int len;
                }nums;
                char *str; /* for .string, .entry and .extern */
            } data; /* the data of the command; unimplemented */
		} request;
		struct /* a command */
        {
			int opcode; /* the location in the opcodes table */
			parameter *p1, *p2; /* the two parameters */
		} command;
        char *error; /* should be changed to an int to store error codes */
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
    enum {NO, YES} makeOb;  /* if syntax error was found */
    enum {NO, YES} makeExt; /* if .extern was found */
    enum {NO, YES} maxeEnt; /* if .entry was found */
} My_File;

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
    int value;
    struct Node *next;
} Symbol;

typedef struct
{
    Symbol *head;
} SymbolList;
