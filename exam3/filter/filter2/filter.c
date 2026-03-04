
#define _GNU_SOURCE

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *make_str(char *arg)
{
	if (!arg)
		return (NULL);

	size_t len = strlen(arg);
	char *str = malloc(len + 1);
	if (!str)
	{
		perror("Error");
		return (NULL);
	}
	memmove(str, arg, len);
	str[len] = '\0';
	return (str);
}

// int main(void)
// {
// 	char *str = "hello world\n";
// 	char *re = make_str(str);
// 	printf("%s", re);
// 	free(re);
// 	return (0);
// }

char *strjoin(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (NULL);

	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char *new = malloc(len1 + len2 + 1);
	if (!new)
	{
		free(s1);
		perror("Error");
		return (NULL);
	}
	memmove(new, s1, len1);
	memmove(new + len1, s2, len2);
	new[len1 + len2] = '\0';
	free(s1);
	return (new);
}

// int main(void)
// {
// 	char *str = malloc(11);
// 	memmove(str, "0123456789", 10);
// 	str[10] = '\0';
// 	char *s2 = "012345";
// 	char *new = strjoin(str, s2);
// 	printf("%s\n", new);
// 	free(new);
// 	return (0);
// }

void print_out(char *haystack, char *needle)
{
	if (!haystack || !needle)
		return ;

	size_t i = 0;
	size_t len_h = strlen(haystack);
	size_t len_n = strlen(needle);
	char *find;
	size_t pos;

	while (i < len_h)
	{
		find = memmem(haystack + i, len_h - i, needle, len_n);
		if (!find)
		{
			write(1, haystack + i, len_h - i);
			break;
		}
		pos = find - haystack;
		if (pos > i)
			write(1, haystack + i, pos - i);
		for (size_t j = 0; j < len_n; j++)
			write(1, "*", 1);
		i = pos + len_n;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2 || argv[1][0] == '\0')
		return (1);
	
	char *str = make_str(argv[1]);
	if (!str)
		return (1);

	char buf[1025];
	ssize_t n;
	char *remain;

	remain = malloc(1);
	if (!remain)
	{
		perror("Error");
		free(str);
		return (1);
	}
	remain[0] = '\0';

	while ((n = read(0, buf, 1024)) > 0)
	{

		buf[n] = '\0';
		remain = strjoin(remain, buf);
		if (!remain)
		{
			free(str);
			return (1);
		}
	}
	if (n < 0)
	{
		perror("Error");
		free(str);
		free(remain);
		return (1);
	}

	print_out(remain, str);
	free(remain);
	free(str);
	return (0);
}
