
/* this file makes a linked list named My_File out of the .as file.
   the My_File is a linked list of My_Line */

#include "my_file.h"

/* makes the full My_File */
My_File * new_file (FILE *asFile)
{
    My_File *file;
    char str_line[MAX_LINE_LENGTH];
    My_Line *curr, *prev;
    
    
    file = (My_File *)malloc(sizeof(My_File));
    
    if (file == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    file->firstLine = NULL;
    file->makeOb = MAKE;
    file->makeExt = DONT;
    file->makeEnt = DONT;
    
    
    prev = NULL;
    while (fgets(str_line, MAX_LINE_LENGTH, asFile))
    {
        curr = new_line(str_line);
        if (curr != NULL)
        {
            if (curr->kind == REQUEST)
            {
                if (curr->statement.request.kind == ENTRY)
                    file->makeEnt = MAKE;
                else if (curr->statement.request.kind == EXTERN)
                    file->makeExt = MAKE;
            }
            else if (curr->kind == ERROR)
            {
                file->makeOb = DONT;
            }
            if (prev == NULL)
            {
                file->firstLine = curr;
                prev = curr;
            }
            else
            {
                prev->next = curr;
                prev = prev->next;
            }
        }
    }
    
    return file;
}

/* makes an My_Line variable out line.
   returns NULL if the line is a comment or empty,
   if the line contains an error, sets the line as an error */
My_Line * new_line (char line[])
{
    char curr[MAX_WORD_LENGTH], *err;
    My_Line *myline;
    int foundError, stat;
    
    if (is_empty(line))
    {
        return NULL;
    }
    
    foundError = 0;
    err = (char *)malloc(MAX_ERROR_LENGTH);
    myline = (My_Line *)malloc(sizeof(My_Line));
    
    if (err == NULL || myline == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    line = getWord(line, curr);
    
    if (is_label(curr))
    {
        strncpy(myline->label, curr, strlen(curr)-1);
        line = getWord(line, curr);
    }
    else
    {
        myline->label[0] = '\0';
        if (is_keyword(curr, ':'))
        {
            strcpy(err, "label can't be a keyword");
            myline->kind = ERROR;
            myline->statement.error = err;
            return myline;
        }
    }
    
    if ((stat = is_command(curr,'\0')) != -1)
    {
        myline->kind = COMMAND;
        myline->statement.command.opcode = stat;
        foundError = set_as_command(myline,line,err);
    }
    else if ((stat = is_request(curr,'\0')) != -1)
    {
        myline->kind = REQUEST;
        myline->statement.request.kind = stat;
        if (stat == ENTRY || stat == EXTERN)
            myline->label[0] = '\0';
        foundError = set_as_request(myline,line,err);
    }
    else
    {
        if (myline->label[0])
            strcpy(err, "no such command or request");
        else
            strcpy(err, "couldn't recognize first word");
        foundError = 1;
    }
    
    if (foundError)/* if contains an error */
    {
        myline->label[0] = '\0';
        myline->kind = ERROR;
        myline->statement.error = err;
    }
    
    return myline;
}

/* finds the first word in line and puts it in "word" 
   returns the new position in line - after the current word */
char * getWord (char *line, char word[])
{
    int i = 0;
    
    skip_spaces(&line);
    
    do
    {
        word[i] = (*line);
        line++;
        i++;
    }while (!is_delimiter(*(line - 1)) && i < MAX_WORD_LENGTH - 1);
    
    if (!is_delimiter(*(line-1)))
    {
        word[0] = '\0';
    }
    else
    {
        word[i] = '\0';
    }
    return line;
}

/* skips spaces in line, if any */
void skip_spaces(char **line)
{
    while((*(*line)) == ' ' || (*(*line)) == '\t')
        (*line)++;
}

/* sets the command parameters and returns 0 if there is no errors in the line,
   set an error messege in "err" and return 1 if there is an error in the line. */
int set_as_command(My_Line *myline, char *line, char err[])
{
    parameter *p1,*p2;
    int i,len;
    
    skip_spaces(&line);
    p1 = (parameter *)malloc(sizeof(parameter));
    p2 = (parameter *)malloc(sizeof(parameter));
    
    if (p1 == NULL || p2 == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    p1->kind = addressingMethod(line);
    len = param_length(line,p1->kind);
    p1->value = (char *)malloc(len+1);
    
    if (p1->value == NULL)
    {
        fprintf(stderr, "error: out of memory\n");
        exit(1);
    }
    
    p1->value[len] = '\0';
    
    switch (p1->kind) {
        case IMMEDIATE:
            line++;
        case REGISTER:
        case DIRECT:
            strncpy(p1->value, line, len);
            line += len;
            break;
        case DISTANCE:
            line++;
            skip_spaces(&line);
            line++;
            skip_spaces(&line);
            i = 0;
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
            {
                p1->value[i] = (*line);
                i++;
                line++;
            }
            skip_spaces(&line);
            line++;
            skip_spaces(&line);
            p1->value[i] = ',';
            i++;
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
            {
                p1->value[i] = (*line);
                i++;
                line++;
            }
            p1->value[len] = '\0';
            skip_spaces(&line);
            line++;
            break;
        case EMPTY:
            p1->value = NULL;
            break;
        case NONE:
            free_parameter(p1);
            free(p2);
            strcpy(err, "syntax problem with first parameter");
            return 1;
            break;
    }
    
    skip_spaces(&line);
    if ((*line) != '\n' && (*line) != '\0')
    {
        if ((*line) != ',')
        {
            free_parameter(p1);
            free(p2);
            strcpy(err, "problem with command's parameter");
            return 1;
        }
        line++;
        skip_spaces(&line);
        p2->kind = addressingMethod(line);
        len = param_length(line,p2->kind);
        p2->value = (char *)malloc(len+1);
        
        if (p2->value == NULL)
        {
            fprintf(stderr, "error: out of memory\n");
            exit(1);
        }
        
        p2->value[len] = '\0';
        
        switch (p2->kind) {
            case IMMEDIATE:
                line++;
            case REGISTER:
            case DIRECT:
                strncpy(p2->value, line, len);
                line += len;
                break;
            case DISTANCE:
                line++;
                skip_spaces(&line);
                line++;
                skip_spaces(&line);
                i = 0;
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
                {
                    p2->value[i] = (*line);
                    i++;
                    line++;
                }
                skip_spaces(&line);
                line++;
                skip_spaces(&line);
                p2->value[i] = ',';
                i++;
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
                {
                    p2->value[i] = (*line);
                    line++;
                }
                p2->value[len] = '\0';
                skip_spaces(&line);
                line++;
                break;
            case EMPTY:
                p2->value = NULL;
            case NONE:
                free_parameter(p1);
                free_parameter(p2);
                strcpy(err, "syntax problem with second parameter");
                return 1;
                break;
        }
        skip_spaces(&line);
        if ((*line) != '\n' && (*line) != '\0')
        {
            free_parameter(p1);
            free_parameter(p2);
            strcpy(err, "too many parameters");
            return 1;
        }
    }
    else
    {
        parameter *tmp;
        p2->kind = EMPTY;
        p2->value = NULL;
        tmp = p2;
        p2 = p1;
        p1 = tmp;
    }
    
    if (fit(myline->statement.command.opcode,p1,FIRST) && fit(myline->statement.command.opcode,p2,SECOND))
    {
        myline->statement.command.p1 = p1;
        myline->statement.command.p2 = p2;
        return 0;
    }
    else
    {
        if (sum(legal_adressing_methods[myline->statement.command.opcode][SECOND],NUMBER_OF_ADDRESSING_METHODS) == 0)
        {
            strcpy(err, "too many parameters");
        }
        else if (sum(legal_adressing_methods[myline->statement.command.opcode][FIRST],NUMBER_OF_ADDRESSING_METHODS) == 0)
        {
            if (p1->kind != EMPTY)
            {
                strcpy(err, "too many parameters");
            }
            else if (p2->kind == EMPTY)
            {
                strcpy(err, "the parameter is missing");
            }
            else
            {
                strcpy(err, "the parameter doesn't fit the command");
            }
        }
        else
        {
            if (p2->kind == EMPTY)
            {
                strcpy(err, "both parameters are missing");
            }
            else if (p1->kind == EMPTY)
            {
                strcpy(err, "second parameter is missing");
            }
            else
            {
                if (fit(myline->statement.command.opcode,p1,FIRST))
                {
                    strcpy(err, "second parameter doesn't fit the command");
                }
                else if (fit(myline->statement.command.opcode,p2,SECOND))
                {
                    strcpy(err, "first parameter doesn't fit the command");
                }
                else
                {
                    strcpy(err, "both parameters don't fit the command");
                }

            }
        }
        
        free_parameter(p1);
        free_parameter(p2);
        return 1;
    }
}

/* sets the request parameters and returns 0 if there is no errors in the line,
   set an error messege in "err" and return 1 if there is an error in the line. */
int set_as_request(My_Line *myline, char *line, char err[])
{
    int len,i,curr,sign;
    len = data_check(line,myline->statement.request.kind);
    
    if (len == -1)
    {
        switch (myline->statement.request.kind) {
            case STRING:
                strcpy(err,"illegal .string parameter");
                break;
            case DATA:
                strcpy(err,"illegal .data parameter");
                break;
            case EXTERN:
                strcpy(err,"illegal .extern parameter");
                break;
            case ENTRY:
                strcpy(err,"illegal .entry parameter");
                break;
        }
        return 1;
    }
    
    switch (myline->statement.request.kind) {
        case DATA:
        {
            int *nums = (int *)malloc(sizeof(int) * len);
            
            if (nums == NULL)
            {
                fprintf(stderr, "error: out of memory\n");
                exit(1);
            }
            
            for (i = 0; i < len; i++)
            {
                skip_spaces(&line);
                curr = 0;
                if ((*line) == '-')
                    sign = -1;
                else if ((*line) == '+')
                    sign = 1;
                else
                {
                    curr = (*line) - '0';
                    sign = 1;
                }
                
                while (isdigit(*(++line)))
                    curr = curr * 10 + (*line) - '0';
                
                skip_spaces(&line);
                line++;
                nums[i] = curr * sign;
            }
            myline->statement.request.data.nums.arr = nums;
            myline->statement.request.data.nums.len = len;
        }
            break;
            
        case STRING:
        {
            char *string = (char *)malloc(len+1);
            
            if (string == NULL)
            {
                fprintf(stderr, "error: out of memory\n");
                exit(1);
            }
            
            skip_spaces(&line);
            line++;
            strncpy(string,line,len);
            string[len] = '\0';
            myline->statement.request.data.str = string;
        }
            break;
            
        default:
        {
            char *symbol = (char *)malloc(len+1);
            
            if (symbol == NULL)
            {
                fprintf(stderr, "error: out of memory\n");
                exit(1);
            }
            
            skip_spaces(&line);
            strncpy(symbol,line,len);
            symbol[len] = '\0';
            myline->statement.request.data.str = symbol;
        }
            break;
    }
    return 0;
}

/* checks the data for errors in case of request from type "kind".
   returns the length of the data if no errors was found,
   return -1 if found an error. */
int data_check(char *param, int kind)
{
    int len = 0;
    skip_spaces(&param);
    switch (kind) {
        case DATA:
        {
            if ((*param) == '+' || (*param) == '-' || isdigit(*param))
            {
                param++;
                len++;
                while ((*param) != '\0' && (*param) != '\n') {
                    if (is_delimiter(*param) && (*param) != ':')
                    {
                        skip_spaces(&param);
                        if ((*param) == '\0' || (*param) == '\n')
                            return len;
                        else
                        {
                            if ((*param) != ',')
                                return -1;
                            param++;
                            skip_spaces(&param);
                            if ((*param) != '+' && (*param) != '-' && !isdigit(*param))
                                return -1;
                            len++;
                        }
                    }
                    else
                    {
                        if ((*param) != '-' && (*param) != '+' && !isdigit(*param))
                            return -1;
                        if (((*param) == '-' || (*param) == '+') && !is_delimiter(*(param-1)))
                            return -1;
                        param++;
                    }
                }
                return len;
            }
            else
                return -1;
        }
            break;
            
        case STRING:
        {
            if ((*param) == '\"')
            {
                param++;
                while ((*param) != '\"' && (*param) != '\0') {
                    param++;
                    len++;
                }
                if ((*param) == '\"')
                {
                    param++;
                    skip_spaces(&param);
                    if ((*param) == '\0' || (*param) == '\n')
                        return len;
                }
            }
            return -1;
        }
            break;
            
        default:
        {
            if (isalpha(*param))
            {
                while (isalnum(*param)) {
                    param++;
                    len++;
                }
                skip_spaces(&param);
                if ((*param) == '\0' || (*param) == '\n')
                    return len;
            }
            return -1;
        }
            break;
    }
    return -1;
}

/* returns non-zero value if the line is empty or a comment,
   returns 0 if not */
int is_empty (char *line)
{
    skip_spaces(&line);
    return ((*line) == ';' || (*line) == '\n' || (*line) == '\0');
}

/* checks if the word is a request.
   returns the index of the request if the word is a command,
   returns -1 if not */
int is_request (char *word, char del)
{
    int i;
    char req[MAX_REQ_LEN + 2]; /* +2 for del and '\0' */
    
    if (del)
    {
        
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,requests[i]);
            req[strlen(requests[i])] = del;
            req[strlen(requests[i])+1] = '\0';
            if (!(strcmp(word,req)))
                return i;
        }
    }
    else
    {
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,requests[i]);
            req[strlen(requests[i])] = ' ';
            req[strlen(requests[i])+1] = '\0';
            if (!(strcmp(word,req)))
                return i;
        }
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,requests[i]);
            req[strlen(requests[i])] = '\t';
            req[strlen(requests[i])+1] = '\0';
            if (!(strcmp(word,req)))
                return i;
        }
    }
    return -1;
}

/* returns 1 if the word is a label
   returns 0 if not.
   label must be followed by ':' */
int is_label (char *word)
{
    if (isalpha(*word))
    {
        char *curr;
        int count = 1;
        curr = word + 1;
        while (*curr && *curr != ':')
        {
            if (!isalnum(*curr))
            {
                return 0;
            }
            count++;
            curr++;
        }
        if ((count <= MAX_SYMBOL_NAME) && !is_keyword(word,':'))
        {
            return 1;
        }
    }
    return 0;
}

/* returns non-zero value if the word is a keyword followed by "del",
   and returns 0 if not */
int is_keyword (char *word, char del)
{
    return ((is_register(word,del) != -1) || (is_command(word,del) != -1));
}

/* returns non-zero value if the word is a command followed by "del",
   and returns 0 if not */
int is_command (char *word, char del)
{
    int i;
    char com[MAX_COM_LEN + 2]; /* +2 for del and '\0' */
    
    if (del)
    {
        
        for (i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            strcpy(com,opcodes[i]);
            com[strlen(opcodes[i])] = del;
            com[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,com)))
                return i;
        }
    }
    else
    {
        for (i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            strcpy(com,opcodes[i]);
            com[strlen(opcodes[i])] = ' ';
            com[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,com)))
                return i;
        }
        for (i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            strcpy(com,opcodes[i]);
            com[strlen(opcodes[i])] = '\t';
            com[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,com)))
                return i;
        }
        for (i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            strcpy(com,opcodes[i]);
            com[strlen(opcodes[i])] = '\n';
            com[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,com)))
                return i;
        }
    }
    return -1;
}

/* returns non-zero value if the word is a register followed by "del",
   and returns 0 if not */
int is_register (char *word, char del)
{
    int i;
    char reg[MAX_REG_LEN + 2]; /* +2 for del and '\0' */
    
    for (i = 0; i < NUMBER_OF_REGISTERS; i++)
    {
        strcpy(reg,registers[i]);
        reg[strlen(registers[i])] = del;
        reg[strlen(registers[i])+1] = '\0';
        if (!(strcmp(word,reg)))
            return i;
    }
    return -1;
}

/* returns 1 if the char is a delimiter from DELIMITERS in definitions,
   and returns 0 if not */
int is_delimiter (char ch)
{
    char *curr = DELIMITERS;
    
    if (ch == '\0')
        return 1;
    
    while (*curr)
    {
        if (*curr == ch)
            return 1;
        curr++;
    }
    
    return 0;
}

/* returns the addressing method of the string line */
int addressingMethod (char *line)
{
    if ((*line) == '#')
    {
        line++;
        if ((*line) == '-' || (*line) == '+' || isdigit(*line))
        {
            line++;
            while (isdigit(*line))
                line++;
            skip_spaces(&line);
            if ((*line) == '\n' || (*line) == '\0' || (*line) == ',')
                return IMMEDIATE;
        }
        return NONE;
    }
    else if ((*line) == '~')
    {
        line++;
        if ((*line) == '(')
        {
            line++;
            skip_spaces(&line);
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
            {
                if (!isalnum(*line))
                {
                    return NONE;
                }
                line++;
            }
            skip_spaces(&line);
            if ((*line) == ',')
            {
                line++;
                skip_spaces(&line);
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
                {
                    if (!isalnum(*line))
                    {
                        return NONE;
                    }
                    line++;
                }
                skip_spaces(&line);
                if ((*line) == ')')
                {
                    line++;
                    skip_spaces(&line);
                    if ((*line) == ',' || (*line) == '\n' || (*line) == '\0')
                        return DISTANCE;
                }
            }
        }
    }
    else if ((*line) == 'r')
    {
        if ((*(line+1)) >= '0' && (*(line+1)) <= '7' && (*(line+2)) != ':' && is_delimiter(*(line+2)))
        {
            line += 2;
            skip_spaces(&line);
            if ((*line) == ',' || (*line) == '\n' || (*line) == '\0')
                return REGISTER;
        }
    }
    if (isalpha(*line))
    {
        line++;
        while ((*line) && (*line) != '\n' && (*line) != ' ' && (*line) != '\t' && (*line) != ',')
        {
            if (!isalnum(*line))
            {
                return NONE;
            }
            line++;
        }
        skip_spaces(&line);
        if ((*line) == '\0' || (*line) == '\n' || (*line) == ',')
        {
            return DIRECT;
        }
    }
    else if ((*line) == '\n' || (*line) == '\0')
    {
        return EMPTY;
    }
    return NONE;
}

/* returns the length of the parameter in case of command from type "kind" */
int param_length (char *param, int kind)
{
    int count;
    count = 0;
    skip_spaces(&param);
    switch (kind) {
        case IMMEDIATE:
            param += 2;
            count = 1;
            while (isdigit(*param))
            {
                count++;
                param++;
            }
            break;
        case DISTANCE:
            param += 2;
            skip_spaces(&param);
            param++;
            count++;
            while (isalnum(*param))
            {
                count++;
                param++;
            }
            skip_spaces(&param);
            param++;
            skip_spaces(&param);
            count += 2;
            param++;
            while (isalnum(*param))
            {
                count++;
                param++;
            }
            break;
        case DIRECT:
            param++;
            count = 1;
            while (isalnum(*param))
            {
                count++;
                param++;
            }
            break;
        case REGISTER:
            return MAX_REG_LEN;
            break;
    }
    
    return count;
}

/* returns 1 if the parameter param fit the command, and 0 if not. */
int fit (int command, parameter *param, int position)
{
    if (param->kind == EMPTY)
    {
        if (sum(legal_adressing_methods[command][position],NUMBER_OF_ADDRESSING_METHODS) == 0)
            return 1;
        else
            return 0;
    }
    else
    {
        return legal_adressing_methods[command][position][param->kind];
    }
}

/* returns the sum of the array arr */
int sum (int arr[], int len)
{
    int i,count;
    count = 0;
    for (i = 0; i < len; i++)
        count += arr[i];
    return count;
}

/* frees the file "file" */
void free_file(My_File *file)
{
    if (file->firstLine)
    {
        My_Line *curr, *post;
        curr = file->firstLine;
        post = curr->next;
        while (post)
        {
            free_line(curr);
            curr = post;
            post = post->next;
        }
        free_line(curr);
    }
    free(file);
}

/* frees the line "line" */
void free_line(My_Line *line)
{
    switch(line->kind)
    {
        case COMMAND:
            free_parameter(line->statement.command.p1);
            free_parameter(line->statement.command.p2);
            break;
        case REQUEST:
            switch(line->statement.request.kind)
        {
            case STRING:
            case ENTRY:
            case EXTERN:
                free(line->statement.request.data.str);
                break;
            case DATA:
                free(line->statement.request.data.nums.arr);
                break;
        }
            break;
        case ERROR:
            free(line->statement.error);
            break;
    }
    free(line);
}

/* frees the parameter p */
void free_parameter(parameter *p)
{
    free(p->value);
    free(p);
}
