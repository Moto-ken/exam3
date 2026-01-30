/*
 * EXERCISE: BROKEN_GNL (Get Next Line)
 *
 * DESCRIPTION:
 * Implement get_next_line that reads line by line from a file descriptor.
 * This version may contain special cases or intentional "bugs".
 */

#include "get_next_line.h"

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

size_t ft_strlen(char *s)
{
	if (!s)
		return 0;
	size_t i = 0;
	while (s[i])
		i++;
	return (i);
}

char *ft_strdup(char *str)
{
	char *res;
	size_t len;
	size_t i = 0;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char *ft_strjoin(char *s1, char *s2)
{
	size_t	total;
	size_t	i;
	size_t	s1len;
	size_t	s2len;
	char	*res;

	i = 0;
	if (!s1)
		return ft_strdup(s2);
	if (!s2)
		return ft_strdup(s1);
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	total = s1len + s2len;
	res = malloc(total + 1);
	if (!res)
		return (NULL);
	while (i < s1len)
	{
		res[i] = s1[i];
		i++;
	}
	while (i < total)
	{
		res[i] = s2[i - s1len];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char *extract_line(char *str)
{
    char	*res;
    size_t	i = 0;

	if (!str || !*str)
		return NULL;
    while (str[i] && str[i] != '\n')
        i++;
    if (str[i] == '\n')
        i++;
    res = malloc(i + 1);
    if (!res)
        return NULL;
    i = 0;
    while (str[i] && str[i] != '\n')
    {
        res[i] = str[i];
        i++;
    }
    if (str[i] == '\n')
        res[i++] = '\n';
    res[i] = '\0';
    return res;
}

char *update_stash(char *stash, char *res)
{
	size_t	len;
	size_t	stash_len;
	char	*new_stash;

	if (!stash)
		return NULL;
	len = ft_strlen(res);
	stash_len = ft_strlen(stash);
	if (len >= stash_len)
	{
		free(stash);
		return NULL;
	}
	new_stash = ft_strdup(stash + len);
	if (!new_stash)
	{
		free(stash);
		return(NULL);
	}
	free(stash);
	return new_stash;
}

char *get_next_line(int fd)
{
	static char *stash;
	char buf[BUFFER_SIZE + 1];
	ssize_t re;
	char *tmp;
	char *res;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stash)
		stash = ft_strdup("");
	while (!(ft_strchr(stash, '\n')))
	{
		re = read(fd, buf, BUFFER_SIZE);
		if (re <= 0)
			break;
		buf[re] = '\0';
		tmp = stash;
		stash = ft_strjoin(stash, buf);
		if (!stash)
		{
			free(tmp);
			return (NULL);
		}
		free(tmp);
	}
	// if (!*stash)
	// {
	// 	free(stash);
	// 	stash = NULL;
	// 	return (NULL);
	// }
	res = extract_line(stash);
	if (!res)
	{
		free (stash);
		stash = NULL;
		return (NULL);
	}
	stash = update_stash(stash, res);
	return (res);
}
