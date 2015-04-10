
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef ASSEMBLY

#define ASSEMBLY ".as"
#define OBJECT ".ob"
#define EXTERNALS ".ext"
#define ENTRIES ".ent"
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
#define MAX_NUM 4096 /* 2 in power of 12 - we have 12 bits */
#define NUMBER_OF_COMMANDS 16
#define NUMBER_OF_REGISTERS 8
#define NUMBER_OF_REQUESTS 4
#define NUMBER_OF_ADDRESSING_METHODS 4
#define FIRST_LINE 100
#define DELIMITERS " \t,:\n"
enum {FIRST, SECOND, THIRD}; /* group number */
enum Addressing {IMMEDIATE, DIRECT, DISTANCE, REGISTER, NONE, ERROR}; /* Addressing methods */
enum Build {MAKE,NOT}; /* make output file or not */

extern char *opcodes[];
extern int legal_adressing_methods[NUMBER_OF_COMMANDS][2][NUMBER_OF_ADDRESSING_METHODS];
extern char *requests[];
extern char *registers[];

typedef struct
{
    enum Addressing kind;
    char *value;
}parameter;

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
            } data;
        } request;
        struct /* a command */
        {
            int opcode; /* the location in the opcodes table */
            parameter *p1, *p2; /* the two parameters */
        } command;
        char *error;
    } statement;
    struct line *next;
} My_Line;

typedef struct
{
    My_Line *firstLine;
    enum Build makeOb;  /* if syntax error was found */
    enum Build makeExt; /* if .extern was found */
    enum Build makeEnt; /* if .entry was found */
} My_File;

typedef union {
    unsigned short int value;
    struct {
        unsigned int mode   :2; /* E/R/A */
        unsigned int target :2; /* target operand Addressing method*/
        unsigned int source :2; /* source operand Addressing method*/
        unsigned int opcode :4;
        unsigned int group  :2; /* the amount of operands for this instruction */
    } bits;
}instr_h; /* the instruction header - the first word of each instruction */

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

#endif
