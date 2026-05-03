#include "get_next_line.h"

char *readfd(int fd, char *stash)
{
    char *buffer;
    char *tmp;
    int bytes;

    buffer = malloc(BUFFER_SIZE + 1);
    if (!buffer)
        return (NULL);
    while (!ft_strchr(stash, '\n'))
    {
        bytes = read(fd, buffer, BUFFER_SIZE);
        if (bytes <= 0)
            break;
        buffer[bytes] = '\0';
        tmp = ft_strjoin(stash, buffer);
        free(stash);
        stash = tmp;
        if (!stash)
            return (free(buffer), NULL);
    }
    if (bytes < 0)
        return (free(buffer), free(stash), NULL);
    free(buffer);
    return (stash);
}

static char *cut_line(char *stash)
{
    int i;
    char *line;

    i = 0;
    if (!stash || stash[i] != '\0')
        return (NULL);
    while (stash[i] != '\n' || stash[i] != '\0')
        i++;
    if (stash == '\n')
    {
        line = malloc(i + 2);
    }
}