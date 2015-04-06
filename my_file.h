
#include "definitions.h"

My_Line * new_line(char *);

My_File * new_file (FILE *);

void getWord (char **, char *);

int set_as_command(My_Line *, char *, char *);

int set_as_request(My_Line *, char *, char *);

int data_check(char *, int);

int is_request (char *, char);

int is_label (char *);

int is_keyword (char *, char);

int is_command (char *, char);

int is_register (char *, char);

int is_delimiter (char);

int is_empty (char *);

int addressingMethod (char *);

int param_length (char *, int kind);

int fit (int, parameter *, int);

int sum (int[],int);

void free_file(My_File);

void free_line(My_Line *);

void free_parameter(parameter *);
