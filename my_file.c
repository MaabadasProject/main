
#include "my_file.h"
#define skip_spaces(line)\
{\
while(*(line) == ' ' || *(line) == '\t')\
(line)++;\
}

My_File * new_file (FILE *asFile)
{
    My_File *file;
    char str_line[MAX_LINE_LENGTH];
    My_Line *curr, *prev;
    
    
    file = (My_File *)malloc(sizeof(My_File));
    file->firstLine = NULL;
    file->makeOb = MAKE;
    file->makeExt = NOT;
    file->makeEnt = NOT;
    
    
    prev = NULL;
    while (!feof(asFile)) {
        curr = new_line(fgets(str_line, MAX_LINE_LENGTH, asFile));
        if (curr->kind == Request)
        {
            if (curr->statement.request.kind == ENTRY)
                file->makeEnt = MAKE;
            else if (curr->statement.request.kind == EXTERN)
                file->makeExt = MAKE;
        }
        else if (curr->kind == Error)
        {
            file->makeOb = NOT;
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
    
    return file;
}

My_Line * new_line (char line[])
{
    char curr[MAX_WORD_LENGTH], *err;
    My_Line *myline;
    int foundError, stat;
    
    foundError = 0;
    err = (char *)malloc(MAX_ERROR_LENGTH);
    myline = (My_Line *)malloc(sizeof(My_Line));
    getWord(&line, curr);
    
    if (is_label(curr))
    {
        strncpy(myline->label, curr, strlen(curr)-1);
        getWord(&line, curr);
    }
    else
    {
        myline->label[0] = '\0';
        if (is_keyword(curr, ':'))
        {
            strcpy(err, "label can't be a keyword");
            myline->kind = Error;
            myline->statement.error = err;
            return myline;
        }
    }
    
    if ((stat = is_command(curr,'\0')) != -1)
    {
        myline->kind = Command;
        myline->statement.command.opcode = stat;
        foundError = set_as_command(myline,line,err);
    }
    else if ((stat = is_request(curr,'\0')) != -1)
    {
        myline->kind = Request;
        myline->statement.request.kind = stat;
        if (stat == ENTRY || stat == EXTERN)
            myline->label[0] = '\0';
        foundError = set_as_request(myline,line,err);
    }
    else if (is_empty(curr)) /* if "line" is empty or comment */
    {
        free(myline);
        free(err);
        return NULL;
    }
    else
    {
        strcpy(err, "couldn't recognize first word");
        foundError = 1;
    }
    
    if (foundError)/* if contains an error */
    {
        myline->label[0] = '\0';
        myline->kind = Error;
        myline->statement.error = err;
    }
    
    return myline;
}

void getWord (char **line, char word[])
{
    int i = 0;
    
    skip_spaces(*line);
    
    do
    {
        word[i] = (**line);
        (*line)++;
        i++;
    }while (!is_delimiter(*(*line - 1)) && i < MAX_WORD_LENGTH - 1);
    
    if (!is_delimiter(*(*line-1)))
    {
        word[0] = '\0';
    }
    else
    {
        word[i] = '\0';
    }
    
}

int set_as_command(My_Line *myline, char *line, char err[])
{
    parameter *p1,*p2;
    int i,len;
    
    skip_spaces(line);
    p1 = (parameter *)malloc(sizeof(parameter));
    p2 = (parameter *)malloc(sizeof(parameter));
    
    p1->kind = addressingMethod(line);
    len = param_length(line,p1->kind);
    p1->value = (char *)malloc(len+1);
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
            skip_spaces(line);
            line++;
            skip_spaces(line);
            i = 0;
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
            {
                p1->value[i] = (*line);
                i++;
                line++;
            }
            skip_spaces(line);
            line++;
            skip_spaces(line);
            p1->value[i] = ',';
            i++;
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
            {
                p1->value[i] = (*line);
                line++;
            }
            p1->value[len] = '\0';
            skip_spaces(line);
            line++;
            break;
        case NONE:
            p1->value = NULL;
        case ERROR:
            free_parameter(p1);
            free(p2);
            strcpy(err, "syntax problem with first parameter");
            return 1;
            break;
    }
    
    skip_spaces(line);
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
        skip_spaces(line);
        p2->kind = addressingMethod(line);
        len = param_length(line,p2->kind);
        p2->value = (char *)malloc(len+1);
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
                skip_spaces(line);
                line++;
                skip_spaces(line);
                i = 0;
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
                {
                    p2->value[i] = (*line);
                    i++;
                    line++;
                }
                skip_spaces(line);
                line++;
                skip_spaces(line);
                p2->value[i] = ',';
                i++;
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
                {
                    p2->value[i] = (*line);
                    line++;
                }
                p2->value[len] = '\0';
                skip_spaces(line);
                line++;
                break;
            case NONE:
                p2->value = NULL;
            case ERROR:
                free_parameter(p1);
                free_parameter(p2);
                strcpy(err, "syntax problem with second parameter");
                return 1;
                break;
        }
        skip_spaces(line);
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
        p2->kind = NONE;
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
        free_parameter(p1);
        free_parameter(p2);
        strcpy(err, "parameters dosn't fit command");
        return 1;
    }
}

int set_as_request(My_Line *myline, char *line, char err[])
{
    int len,i,curr,sign;
    len = data_check(line,myline->statement.request.kind);
    
    if (len == -1)
    {
        strcpy(err,"illegal request parameter");
        return 1;
    }
    
    switch (myline->statement.request.kind) {
        case DATA:
        {
            int *nums = (int *)malloc(sizeof(int) * len);
            for (i = 0; i < len; i++)
            {
                skip_spaces(line);
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
                
                skip_spaces(line);
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
            skip_spaces(line);
            line++;
            strncpy(string,line,len);
            string[len] = '\0';
            myline->statement.request.data.str = string;
        }
            break;
            
        default:
        {
            char *symbol = (char *)malloc(len+1);
            skip_spaces(line);
            strncpy(symbol,line,len);
            symbol[len] = '\0';
            myline->statement.request.data.str = symbol;
        }
            break;
    }
    return 0;
}

int data_check(char *param, int kind)
{
    int len = 0;
    skip_spaces(param);
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
                        skip_spaces(param);
                        if ((*param) == '\0' || (*param) == '\n')
                            return len;
                        else
                        {
                            if ((*param) != ',')
                                return -1;
                            param++;
                            skip_spaces(param);
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
                    skip_spaces(param);
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
                skip_spaces(param);
                if ((*param) == '\0' || (*param) == '\n')
                    return len;
            }
            return -1;
        }
            break;
    }
    return -1;
}

int is_empty (char *line)
{
    skip_spaces(line);
    return ((*line) == ';' || (*line) == '\n' || (*line) == '\0');
}

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

int is_keyword (char *word, char del)
{
    return ((is_register(word,del) != -1) || (is_command(word,del) != -1));
}

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
    }
    return -1;
}

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
            skip_spaces(line);
            if ((*line) == '\n' || (*line) == '\0' || (*line) == ',')
                return IMMEDIATE;
        }
        return ERROR;
    }
    else if ((*line) == '~')
    {
        line++;
        if ((*line) == '(')
        {
            line++;
            skip_spaces(line);
            while ((*line) != ' ' && (*line) != '\t' && (*line) != ',')
            {
                if (!isalnum(*line))
                {
                    return ERROR;
                }
                line++;
            }
            skip_spaces(line);
            if ((*line) == ',')
            {
                line++;
                skip_spaces(line);
                while ((*line) != ' ' && (*line) != '\t' && (*line) != ')')
                {
                    if (!isalnum(*line))
                    {
                        return ERROR;
                    }
                    line++;
                }
                skip_spaces(line);
                if ((*line) == ')')
                {
                    line++;
                    skip_spaces(line);
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
            skip_spaces(line);
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
                return ERROR;
            }
            line++;
        }
        skip_spaces(line);
        if ((*line) == '\0' || (*line) == '\n' || (*line) == ',')
        {
            return DIRECT;
        }
    }
    else if ((*line) == '\n' || (*line) == '\0')
    {
        return NONE;
    }
    return ERROR;
}

int param_length (char *param, int kind)
{
    int count;
    count = 0;
    skip_spaces(param);
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
            skip_spaces(param);
            param++;
            count++;
            while (isalnum(*param))
            {
                count++;
                param++;
            }
            skip_spaces(param);
            param++;
            skip_spaces(param);
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

int fit (int command, parameter *param, int numberOfParameter)
{
    if (param->kind == NONE)
    {
        return (sum(legal_adressing_methods[command][numberOfParameter],NUMBER_OF_ADDRESSING_METHODS) == 0);
    }
    else
    {
        return legal_adressing_methods[command][numberOfParameter][param->kind];
    }
}

int sum (int arr[], int len)
{
    int i,count;
    count = 0;
    for (i = 0; i < len; i++)
        count += arr[i];
    return count;
}

void free_file(My_File file)
{
    if (file.firstLine)
    {
        My_Line *curr, *post;
        curr = file.firstLine;
        post = curr->next;
        while (post)
        {
            free_line(curr);
            curr = post;
            post = post->next;
        }
        free_line(curr);
    }
    free(&file);
}

void free_line(My_Line *line)
{
    free(line->label);
    switch(line->kind)
    {
        case Command:
            free_parameter(line->statement.command.p1);
            free_parameter(line->statement.command.p2);
            break;
        case Request:
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
        case Error:
            free(line->statement.error);
            break;
    }
    free(line);
}

void free_parameter(parameter *p)
{
    free(p->value);
    free(p);
}
