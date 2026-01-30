
#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

char *strjoin(char *haystack, char *needle)
{
	if (!needle)
	{
		free(haystack);
		return (NULL);
	}

	size_t hay_c = strlen(haystack);
	size_t nee_c = strlen(needle);
	char *result = malloc(hay_c + nee_c + 1);
	if (!result)
	{
		free(haystack);
		perror("Error");
		return (NULL);
	}
	memmove(result, haystack, hay_c);
	memmove(result + hay_c, needle, nee_c);
	result[hay_c + nee_c] = '\0';
	free(haystack);
	return (result);
}

void print_str(char *haystack, char *needle)
{
	int i = 0;
	char *pos;
	size_t hay_c = strlen(haystack);
	size_t nee_c = strlen(needle);
	while (haystack[i])
	{
		pos = memmem(haystack + i, hay_c - i, needle, nee_c);
		if (pos)
		{
			int bef = pos - (haystack + i);
			write(1, haystack + i, bef);
			size_t j = 0;
			while (j < nee_c)
			{
				write(1, "*", 1);
				j++;
			}
			i = i + nee_c + bef;
		}
		else
		{
			write(1, haystack + i, 1);
			i++;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2 || argv[1][0] == '\0')
		return (1);

	char *needle = argv[1];
	ssize_t n;
	char buf[11];
	char *haystack = malloc(1);
	if (!haystack)
	{
		perror("Error");
		return (1);
	}
	haystack[0] = '\0';

	while ((n = read(0, buf, 10)) > 0)
	{
		buf[n] = '\0';
		haystack = strjoin(haystack, buf);
		if (!haystack)
			return (1);
	}
	if (n < 0)
	{
		free(haystack);
		return (1);
	}
	print_str(haystack, needle);
	free(haystack);
	return (0);
}
