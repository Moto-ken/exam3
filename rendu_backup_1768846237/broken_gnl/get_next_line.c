
#include "get_next_line.h"

size_t ft_strlen(char *str)
{
	size_t i = 0;
	if (!str)
		return (0);

	while (str[i])
		i++;
	return (i);
}

char *ft_strchr(char *str, int c)
{
	size_t i = 0;
	if (!str)
		return (NULL);

	while (str[i])
	{
		if (str[i] == c)
			return (str + i);
		i++;
	}
	return (NULL);
}

void *ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *d = dest;
	unsigned const char *s = src;
	size_t i = 0;

	while (n > i)
	{
		d[i] = s[i];
		i++;
	}
	return (d);
}

char *strjoin(char *stash, char *buf)
{
	if (!buf)
	{
		free(stash);
		return (NULL);
	}
	char *new;
	size_t s_len = ft_strlen(stash);
	size_t b_len = ft_strlen(buf);

	new = malloc(s_len + b_len + 1);
	if (!new)
	{
		free(stash);
		return (NULL);
	}
	ft_memmove(new, stash, s_len);
	ft_memmove(new + s_len, buf, b_len);
	new[s_len + b_len] = '\0';
	free(stash);
	return (new);
}

char *extract_line(char *stash)
{
	if (!stash)
		return (NULL);
	char *re;
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
	re = malloc(i + 1);
	if (!re)
		return (NULL);
	ft_memmove(re, stash, i);
	re[i] = '\0';
	return (re);
}

// int main(void)
// {
// 	char *str = "hello\nworld";
// 	char *re = extract_line(str);
// 	printf("%s", re);
// 	free(re);
// 	return (0);
// }

char *update_stash(char *stash, char *re)
{
	char *new;
	size_t l1 = ft_strlen(stash);
	size_t l2 = ft_strlen(re);

	new = malloc(l1 - l2 + 1);
	if (!new)
	{
		free(stash);
		free(re);
		stash = NULL;
		return (NULL);
	}
	ft_memmove(new, stash + l2, l1 - l2);
	new[l1 - l2] = '\0';
	free(stash);
	return (new);
}

// int main(void)
// {
// 	char *s1;
// 	char *s2;

// 	s1 = malloc(11);
// 	if (!s1)
// 		return (1);
// 	ft_memmove(s1, "0123456789", 10);
// 	s1[10] = '\0';
// 	s2 = malloc(4);
// 	if (!s2)
// 		return (1);
// 	ft_memmove(s2, "012", 3);
// 	s2[3] = '\0';
// 	char *re = update_stash(s1, s2);
// 	printf("%s", re);
// 	free(re);
// 	free(s2);
// 	return (0);
// }


char *get_next_line(int fd)
{
	if (fd < 0 || BUFFER_SIZE <=0)
		return (NULL);

	static char *stash;
	ssize_t n;
	char buf[BUFFER_SIZE + 1];
	char *re;

	if (!stash)
	{
		stash = malloc(1);
		if (!stash)
			return (NULL);
		stash[0] = '\0';
	}
	while (!ft_strchr(stash, '\n'))
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
	re = extract_line(stash);
	if (!re || !*re)
	{
		free(stash);
		free(re);
		stash = NULL;
		return (NULL);
	}
	stash = update_stash(stash, re);
	if (!stash)
		return (NULL);
	return (re);
}


// int main(void)
// {
// 	char *re;
// 	int fd = open("text.txt", O_RDONLY);
// 	if (fd < 0)
// 		return (1);
	
// 	while (re = get_next_line(fd))
// 	{
// 		printf("%s", re);
// 		free(re);
// 	}
// 	close(fd);
// 	return (0);
// }
