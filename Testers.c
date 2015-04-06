
#include "Testers.h"
/*
 int (*test[NUM_OF_TESTS])(void) = {
 
 };*/

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

int legal_adressing_methods[NUMBER_OF_COMMANDS][2][NUMBER_OF_ADDRESSING_METHODS] =
/*source  ||  destination*/
{{{1,1,1,1},	{0,1,0,1}},
    {{1,1,1,1},		{1,1,1,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{1,1,1,1},		{0,1,0,1}},
    {{0,0,0,0},		{0,0,0,1}},
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
    {{0,0,0,0},		{0,0,0,0}}};

char *requests[NUMBER_OF_REQUESTS] =
{ ".data"
    , ".string"
    , ".entry"
    , ".extern"};

char *registers[NUMBER_OF_REGISTERS] =
{ "r0"
    , "r1"
    , "r2"
    , "r3"
    , "r4"
    , "r5"
    , "r6"
    , "r7"};

int main()
{
    char input[100];
    My_Line *output;
    
    fgets(input, 100, stdin);
    
    output = new_line(input);
    
    if (output->label[0])
        printf("label:\t%s\n",output->label);
    
    switch (output->kind) {
        case Command:
        {
            printf("command - %s:\n",opcodes[output->statement.command.opcode]);
            printf("param 1:\t");
            switch (output->statement.command.p1->kind) {
                case REGISTER:
                    printf("register - ");
                    break;
                case IMMEDIATE:
                    printf("immediate - ");
                    break;
                case DIRECT:
                    printf("direct - ");
                    break;
                case DISTANCE:
                    printf("distance - ");
                    break;
                case NONE:
                    printf("none - ");
                    break;
                case ERROR:
                    break;
            }
            printf("%s\n",output->statement.command.p1->value);
            printf("param 2:\t");
            switch (output->statement.command.p2->kind) {
                case REGISTER:
                    printf("register - ");
                    break;
                case IMMEDIATE:
                    printf("immediate - ");
                    break;
                case DIRECT:
                    printf("direct - ");
                    break;
                case DISTANCE:
                    printf("distance - ");
                    break;
                case NONE:
                    printf("none - ");
                    break;
                case ERROR:
                    break;
            }
            printf("%s\n",output->statement.command.p2->value);
        }
            break;
            
        case Request:
        {
            printf("request - ");
            switch (output->statement.request.kind) {
                case DATA:
                {
                    int i;
                    printf("data:\t");
                    for (i = 0; i < output->statement.request.data.nums.len; i++)
                        printf("%d,",(output->statement.request.data.nums.arr)[i]);
                    printf("\n");
                }
                    break;
                case EXTERN:
                    printf("extern:\t%s\n",output->statement.request.data.str);
                    break;
                case STRING:
                    printf("string:\t\"%s\"\n",output->statement.request.data.str);
                    break;
                case ENTRY:
                    printf("entry:\t%s\n",output->statement.request.data.str);
                    break;
            }
        }
            break;
        case Error:
            printf("Error:\t%s\n",output->statement.error);
            break;
    }
    return 0;
}
/*
 main()
 {
 int testNumber,res,finRes;
 
 printf("Please enter the test number:\n\t0. all project\n\t1. parsing file test\n\t2. symbol list test\n");
 printf("\nenter anything else to stop testing.\n");
 
 finRes = 0;
 
 while(scanf("%d",&testNumber) == 1 && testNumber >= 0 && testNumber < NUM_OF_TESTS)
 {
 res = (*(test[testNumber]))();
 printf("\nnumber of errors on this test: %d\n",res);
 finRes += res;
 }
 
 printf("\ntotal number of errors: %d\n",finRes);
 
 return 0;
 }
 
 int parsing()
 {
 int errorCount;
 FILE *testOn;
 My_file *file1,*file2,*file3;
 
 errorCount = 0;
 file2 = good();
 file3 = bad();
 
 if (testOn = fopen(good.as,READ))
 {
 file1 = new_file(testOn);
 //TODO: today
 fclose(testOn);
 }
 
 return errorCount;
 }
 */
