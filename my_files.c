
#include "my_file.h"

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
    /* TODO: write when it's time */
}

My_File new_file (FILE *asFile)
{
    /* TODO: write when it's time */
}

My_Line * new_line (/* TODO: write when it's time */)
{
    /* TODO: write when it's time */
}
