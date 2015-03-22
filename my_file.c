
#include "my_file.h"

#define skip_spaces(line)\
{\
    while(*(line) == ' ' || *(line) == '\t')\
    	(line)++;\
}

My_File new_file (FILE *asFile)
{
    My_File file;
    char str_line[MAX_LINE_LENGTH];
    My_Line *curr, *prev;
    
    
    &file = (My_File *)malloc(sizeof(My_File));
    file.firstLine = NULL;
    file.makeOb = YES;
    file.makeExt = NO;
    file.makeEnt = NO;
    
	
    prev = NULL;
	while (!feof(asFile)) {
		curr = new_line(fgets(str_line, MAX_LINE_LENGTH, asFile));
		if (curr->kind == Request)
		{
			if (curr->statement.request.kind == ENTRY)
				file.makeEnt = YES;
			else if (curr->statement.request.kind == EXTERN)
				file.makeExt = YES;
		}
		else if (curr->kind == Error)
		{
			file.makeOb = NO;
		}
		if (prev == NULL)
		{
			file.firstLine = curr;
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
    char curr[MAX_WORD_LENGTH], err[MAX_ERROR_LENGTH];
    My_Line *myline;
    int foundError, stat;
    
    foundError = 0;
    err = (char *)malloc(MAX_ERROR_LENGTH);
    myline = (My_Line *)malloc(sizeof(My_Line));
    getWord(&line, curr);
    
    if (is_label(curr))
    {
        strcpy_label(myline->label, curr);
        getWord(&line, curr);
    }
    else
    {
        myline->label[0] = '\0';
    }
    
    if ((stat = is_command(curr,NULL)) != -1)
    {
        myline->kind = Command;
        myline->statement.command.opcode = stat;
        foundError = set_as_command(myline,line,err);
    }
    else if ((stat = is_request(curr,NULL)) != -1)
    {
        myline->kind = Request;
        myline->statement.request.kind = stat;
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
        strcpy(err, "error: couldn't recognize command or request");
        foundError = 1;
    }
    
    if (foundError)/* if contains an error */
    {
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
    
    if (!is_delimiter(**line))
    {
        word[0] = '\0';
    }
    else
    {
        word[i] = **line;
    }
    
}

int set_as_command(My_Line *myline, char *line, char err[])
{
    
}

int set_as_request(My_Line *myline, char *line, char err[])
{
    int len,i,curr,sign;
    len = data_check(line,myline->statement.request.kind);
    
    if (len == -1)
    {
        strcpy(err,"error: illegal request parameter");
        return 1;
    }
    
    switch (myline->statement.request.kind) {
        case DATA:
        {
            int nums[] = (int *)malloc(sizeof(int) * len);
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
                    len++
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
                param++;
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

int is_request (char *word, char del)
{
    int i;
    char req[MAX_REQ_LEN + 2]; /* +2 for del and '\0' */
    
    if (del)
    {
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,opcodes[i]);
            req[strlen(opcodes[i])] = del;
            req[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,req)))
                return i;
        }
    }
    else
    {
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,opcodes[i]);
            req[strlen(opcodes[i])] = ' ';
            req[strlen(opcodes[i])+1] = '\0';
            if (!(strcmp(word,req)))
                return i;
        }
        for (i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            strcpy(req,opcodes[i]);
            req[strlen(opcodes[i])] = '\t';
            req[strlen(opcodes[i])+1] = '\0';
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

void free_file(My_File file)
{
    if (file.firstLine)
    {
        My_Line *curr, *post;
        curr = file.firstLine;
        post = curr.next;
        while (post)
        {
            free_line(curr);
            curr = post;
            post = post.next;
        }
        free_line(curr);
    }
    free(file);
}

void free_line(My_Line *line)
{
	/* wait 'till we write the new_line func */
	
	/*My_Line ptr;
	
	while (line != NULL) {
		free(line->label);
		switch(line->data) {
			case Command:
				free(line->data.command.p1);
				free(line->data.command.p2);
				break;
			case Request:
				// free(line->data.request.data) // data unimplemented; might need a free in the future
				break;
		}
		ptr = line->next;
		free(line);
		line = ptr;
	}*/
}


/* writing files */

instr_h_b *get_header(Line *line)
{
	instr_h_b *h;
	int amount_of_parameters = 0;
	
	h = (*instr_h_b) malloc(sizeof(instr_h_b);
	switch(line->kind)
	{
		case Request:
		case Error:
			h->mode=0; break;
		case Command:
			/* first figure out the A/R/E */
			if(line->statement.command->p1)
			{
				h.target = line->statement.command->p1->kind;
				amount_of_parameters++;
			}
			if(line->statement.command->p2)
			{
				h.source = line->statement.command->p2->kind;
				amount_of_parameters++;
			}
			h.opcode = line->statement.command.opcode;
			h.group = amount_of_parameters;
	}
	
	return istr_h_b;
}

void write_header(FILE *f, Line *line)
{
	instr_h_b *h;
	h = get_header(line);
	fwrite(h, 2 /* maybe? */, 1, f);
	free(instr_h_b);
}

	
