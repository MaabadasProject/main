
#include "my_file.h"


My_File new_file (FILE *asFile)
{
    My_File file;
    char *str_line;
    My_Line *curr, *prev;
    
    &file = (My_File *)malloc(sizeof(My_File));
    file.firstLine = NULL;
    file.makeOb = YES;
    file.makeExt = NO;
    file.makeEnt = NO;
    
    fseek(asFile,0L,SEEK_SET);
    if (getLine(&str_line))
    {
        prev = new_line(str_line);
        if (prev->kind == Request)
        {
            if (prev->data.request.kind == ENTRY)
                file.makeEnt = YES;
            else if (prev->data.request.kind == EXTERN)
                file.makeExt = YES;
        }
        else if (prev->kind == Error)
        {
			/* isn't there a better way to handle errors? or is it not an error? */
            file.makeOb = NO;
        }
        file.firstLine = prev;
        free(str_line);
        while (getLine(&str_line)) /* why do you work on prev but not on curr in this loop? prev is already processed and the last line will be skipped. */
        {                          /* seems like you should change the names and get one while or do-while loop */
            curr = new_line(str_line);
            
            if (prev->kind == Request) 
			                            
            {
                if (prev->data.request.kind == ENTRY)
                    file.makeEnt = YES;
                else if (prev->data.request.kind == EXTERN)
                    file.makeExt = YES;
            }
            else if (prev->kind == Error)
            {
                file.makeOb = NO;
            }
            
            prev->next = curr;
            prev = prev->next;
            free(str_line);
        }
    }
    return file;
}

My_Line * new_line (/* TODO: write when it's time */)
{
    /* TODO: write when it's time */
}

/* what does this function do? document noam */
int getLine(char **line)
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