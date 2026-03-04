
#include "get_next_line.h"

char *ft_strchr(char *str, int c)
{
	if (!str)
		return (NULL);
	size_t i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (str + i);
		i++;
	}
	return (NULL);
}

size_t ft_strlen(char *str)
{
	size_t i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void *ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;
	size_t i = 0;

	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

char *strjoin(char *stash, char *buf)
{

	if (!buf)
	{
		free(stash);
		return (NULL);
	}

	size_t len1 = ft_strlen(stash);
	size_t len2 = ft_strlen(buf);
	char *re = malloc(len1 + len2 + 1);
	if (!re)
	{
		free(stash);
		return (NULL);
	}
	ft_memmove(re, stash, len1);
	ft_memmove(re + len1, buf, len2);
	re[len1 + len2] = '\0';
	free(stash);
	return (re);
}

int found_line(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char *extract_line(char *str)
{
	int pos;

	if (!str || !str[0])
		return NULL;

	pos = found_line(str);
	if (pos >= 0)
	{
		char *re = malloc(pos + 2);
		if (!re)
			return NULL;
		ft_memmove(re, str, pos + 1);
		re[pos + 1] = '\0';
		return re;
	}
	return (strdup(str));
}

char *update_stash(char *str)
{
	int pos = found_line(str);
	// printf("%d\n", pos);
	if (pos < 0)
	{
		free(str);
		return (NULL);
	}
	int len = ft_strlen(str);
	// printf("%d\n", len);
	int remain = len - pos + 1;
	char *re = malloc(remain + 1);
	if (!re)
	{
		free(str);
		return (NULL);
	}
	ft_memmove(re, str + pos + 1, remain);
	re[remain] = '\0';
	free(str);
	return (re);
}

// int main(void)
// {
// 	char *str = malloc(12);
// 	ft_memmove(str, "hello\nworld", 11);
// 	str[11] = '\0';
// 	printf("%s\n", str);
// 	str = update_stash(str);
// 	if (str)
// 		printf("%s\n", str);
// 	free(str);
// 	return (0);
// }

char *get_next_line(int fd)
{
	char *re;
	static char *stash;
	ssize_t n;
	char buf[BUFFER_SIZE + 1];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	
	if (!stash)
	{
		stash = malloc(1);
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	
	while (!(ft_strchr(stash, '\n')))
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n < 0)
		{
			free(stash);
			return (NULL);
		}
		if (n == 0)
			break;
		buf[n] = '\0';
		stash = strjoin(stash, buf);
		if (!stash)
			return (NULL);
	}
	re = extract_line(stash);
	if (!re)
	{
		free(stash);
		return (NULL);
	}
	stash = update_stash(stash);
	return (re);
}


#include <fcntl.h>

int main(void)
{
	int fd = open("../text.txt", O_RDONLY);
	char *re;

	while (re = get_next_line(fd))
	{
		printf("%s", re);
		free(re);
	}
	return (0);
}
