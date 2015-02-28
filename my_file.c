
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
    int foundError = 0;
    
    err = (char *)malloc(MAX_ERROR_LENGTH);
    myline = (My_Line *)malloc(sizeof(My_Line));
    getWord(&line, curr);
    
    if (is_label(curr)) /* if curr is label, it also deletes the trailing ':' */
    {
        strcpy(myline->label, curr);
        getWord(&line, curr);
    }
    else
    {
        myline->label[0] = '\0';
    }
    
    if (is_command(curr))
    {
        
    }
    else if (is_request(curr))
    {
        
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
        myline->label = NULL;
        myline->statement.error = err;
        return myline;
    }
}

void getWord (char **line, char word[])
{
    int i = 0;
    
    skip_spaces(*line);
    
    while ((**line) != ' ' && (**line) != '\t' && (**line) != '\0' && i < MAX_WORD_LENGTH - 1)
    {
        word[i] = (**line);
        (*line)++;
        i++;
    }
    if ((**line) != ' ' && (**line) != '\t' && (**line) != '\0')
    {
        i = 0;
    }
    word[i] = '\0';
}

int is_request (char *word)
{
    /* TODO: write when it's time */
}

int is_command (char *word)
{
    /* TODO: write when it's time */
}

int is_label (char *word)
{
    if (isalpha(*word))
    {
        int count = 1;
        word++;
        while (*word && *word != ':')
        {
            count++;
            word++;
        }
        if ((*word == ':') && (count <= MAX_SYMBOL_NAME))
        {
            return 1;
        }
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
