#include "get_next_line.h"

char *ft_strchr(const char *s, int c)
{
    size_t i;
    unsigned char k;
    unsigned char *p;

    i = 0;
    k = (unsigned char)c;
    p = (unsigned char *)s;
    while (p[i])
    {
        if (p[i] == k)
            return ((char *)&p[i]);
        i++;
    }
    if (k == '\0')
        return ((char *)&p[i]);
    return (NULL);
}

char *ft_strjoin(char *s1, const char *s2)
{
    char *res;
    size_t j;
    size_t i;

    res = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!res)
        return (NULL);
    i = 0;
    while (s1[i])
    {
        res[i] = s1[i];
        i++;
    }
    j = 0;
    while (s2[j])
    {
        res[i + j] = s2[j];
        j++;
    }
    res[i + j] = '\0';
    return (res);
}