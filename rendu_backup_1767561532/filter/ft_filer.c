
#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

char *strjoin(char *stash, char *buf)
{
	if (!stash || !buf)
		return (NULL);

	size_t s1 = strlen(stash);
	size_t s2 = strlen(buf);
	char *new;

	new = malloc(s1 + s2 + 1);
	if (!new)
	{
		free(stash);
		perror("Error");
		return (NULL);
	}
	memmove(new, stash, s1);
	free(stash);
	memmove(new + s1, buf, s2);
	new[s1 + s2] = '\0';
	return (new);
}

void print_stash(char *stash, char *needle)
{
	size_t i = 0;
	size_t len1 = strlen(stash);
	size_t len2 = strlen(needle);
	char *match;

	while (i < len1)
	{
		match = memmem(stash + i, len1 - i, needle, len2);
		if (match)
		{
			size_t j = match - (stash + i);
			write(1, stash + i , j);
			for (size_t k = 0; k < len2; k++)
				write(1, "*", 1);
			i = i + len2 + j;
		}
		else
		{
			write(1, stash + i, 1);
			i++;
		}
	}
}

int main(int argc, char *argv[])
{
    if (argc != 2 || argv[1][0] == '\0')
        return (1);
    
    char *needle;
    size_t len;
    ssize_t n;
    char buf[1025];
	static char *stash;

    len = strlen(argv[1]);
    needle = malloc(len + 1);
    if (!needle)
    {
        perror("Error");
        return (1);
    }
    memmove(needle, argv[1], len);
    needle[len] = '\0';
	// if (!stash)
	// {
		stash = malloc(1);
		if (!stash)
		{
			free(needle);
			perror("Error");
			return (1);
		}
		stash[0] = '\0';
	// }
    while ((n = read(0, buf, 1024)) > 0)
    {
		buf[n] = '\0';
		stash = strjoin(stash, buf);
		if (!stash)
		{
			free(needle);
			return (1);
		}
    }
	if (n < 0)
    {
		free(stash);
		free(needle);
        perror("Error");
        return (1);
    }
	print_stash(stash, needle);
	free(stash);
	free(needle);
    return (0);
}
