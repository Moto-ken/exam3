
#include "get_next_line.h"
#include <fcntl.h>

char *ft_strchr(char *s, int c)
{
	if (!s)
		return (NULL);
	size_t i = 0;

	while (s[i])
	{
		if (s[i] == c)
			return (s + i);
		i++;
	}
	return (NULL);
}

size_t ft_strlen(char *str)
{
	if (!str)
		return (0);
	size_t i = 0;

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
	return (d);
}

char *strjoin(char *s1, char *s2)
{
	if (!s2)
	{
		free(s1);
		return (NULL);	
	}
	size_t len1 = ft_strlen(s1);
	size_t len2 = ft_strlen(s2);
	char *res;

	res = malloc(len1 + len2 + 1);
	if (!res)
	{
		free(s1);
		return (NULL);
	}
	ft_memmove(res, s1, len1);
	ft_memmove(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	free(s1);
	return (res);
}

char *extract_line(char *stash)
{
	char *res;
	size_t i = 0;

	while (stash[i])
	{
		if (stash[i] == '\n')
		{
			i++;
			break;
		}
		i++;
	}
	res = malloc(i + 1);
	if (!res)
		return (NULL);
	ft_memmove(res, stash, i);
	res[i] = '\0';
	return (res);
}

char *update_stash(char *stash, char *res)
{
	size_t s_len = ft_strlen(stash);
	size_t r_len = ft_strlen(res);
	char *new;

	new = malloc(s_len - r_len + 1);
	if (!new)
	{
		free(stash);
		return (NULL);
	}
	ft_memmove(new, stash + r_len, s_len - r_len);
	new[s_len - r_len] = '\0';
	free(stash);
	return (new);
}

char *get_next_line(int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);

	static char *stash;
	ssize_t n;
	char buf[BUFFER_SIZE + 1];
	char *res;

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
			stash = NULL;
			return (NULL);
		}
		if (n == 0)
			break;
		buf[n] = '\0';
		stash = strjoin(stash, buf);
		if (!stash)
			return (NULL);
	}
	res = extract_line(stash);
	if (!res || !*res)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = update_stash(stash, res);
	if (!stash)
		return (NULL);
	return (res);
}


int main(void)
{
	char *result;
	int fd;

	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	while (result = get_next_line(fd))
	{
		printf("%s", result);
		free(result);
	}
	clode(fd);
	return (0);
}
