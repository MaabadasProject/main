
#include "my_file.h"

#define my_getc(line,ch)\
{\
    char c = *(line);\
    while (c == ' ' || '\t')\
        c = *(++(line));\
    ch = c;\
    (line)++;\
}

#define is_error(word) (*(word)) == '\0'

My_File new_file (FILE *asFile)
{
    My_File file;
    char str_line[MAX_LINE_LENGTH];
    My_Line *curr, *prev;
    
    curr = prev = NULL;
    
    &file = (My_File *)malloc(sizeof(My_File));
    file.firstLine = NULL;
    file.makeOb = YES;
    file.makeExt = NO;
    file.makeEnt = NO;
    
    while (fgets(str_line,MAX_LINE_LENGTH,asFile) && !(prev = new_line(str_line)));
    
    if (prev)
    {
        if (prev->kind == Request)
        {
            if (prev->statement.request.kind == ENTRY)
                file.makeEnt = YES;
            else if (prev->statement.request.kind == EXTERN)
                file.makeExt = YES;
        }
        else if (prev->kind == Error)
        {
            file.makeOb = NO;
        }
        file.firstLine = prev;
        while (fgets(str_line,MAX_LINE_LENGTH,asFile))
        {
            if ((curr = new_line(str_line)))
            {
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
                
                prev->next = curr;
                prev = prev->next;
            }
        }
    }
    
    return file;
}

My_Line * new_line (char line[])
{
    char *first = getFirstWord(&line);
    if (*first == ';' || *first == '\n')
    {
        return NULL;
    }
    else
    {
        if (is_error(first))
        {
            
        }
        else if (is_request(first))
        {
            
        }
        else if (is_command(first))
        {
            
        }
        else /* if "first" is label */
        {
            
        }
    }
}

char * getFirstWord (char **line)
{
    int i = 1;
    char *word = (char *)malloc(MAX_SYMBOL_NAME + 1); /* +1 for '\0' */
    my_getc(*line,word[0]);
    
    while (!isspace(**line) && (**line) != '\0' && i < MAX_SYMBOL_NAME)
    {
        word[i] = (**line);
        (*line)++;
        i++;
    }
    if (!isspace(**line) && (**line))
    {
        i = 0;
    }
    word[i] = '\0';
    
    return word;
}

int is_request (char *word)
{
    /* TODO: write when it's time */
}

int is_command (char *word)
{
    /* TODO: write when it's time */
}

void free_file(My_File file)
{
    if (file.firstLine)
    {
        My_Line *curr,*post;
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
	My_Line ptr;
	
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
	}
}
