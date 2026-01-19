
#include <unistd.h>
#include <stdio.h>

size_t count_ele(char *str, int c)
{
	size_t i = 0;
	size_t re = 0;

	while (str[i])
	{
		if (str[i] == c)
			re++;
		i++;
	}
	return (re);
}

int is_balanced(char *s)
{
	int c = 0;
	int i = 0;

	while (s[i])
	{
		if (s[i] == '(')
			c++;
		else if (s[i] == ')')
			c--;
		if (c < 0)
			return (0);
		i++;
	}
	return (c == 0);
}

void	print(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	write(1, s, i);
	write(1, "\n", 1);
}

void dfs(char *s, int i, int rm_s, int rm_e)
{
	if (s[i] == '\0')
	{
		if (rm_s == 0 && rm_e == 0 && is_balanced(s))
			puts(s);
		return;
	}
	if (s[i] == '(' && rm_s > 0)
	{
		s[i] = ' ';
		dfs(s, i + 1, rm_s - 1, rm_e);
		s[i] = '(';
	}
	if (s[i] == ')' && rm_e > 0)
	{
		s[i] = ' ';
		dfs(s, i + 1, rm_s, rm_e - 1);
		s[i] = ')';
	}
	dfs(s, i + 1, rm_s, rm_e);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return (1);
	
	size_t i = 0;

	while (argv[1][i])
	{
		if (argv[1][i] != '(' && argv[1][i] != ')')
			return (1);
		i++;
	}
	i = 0;
	int rm_s = 0;
	int rm_e = 0;
	while (argv[1][i])
	{
		if (argv[1][i] == '(')
			rm_s++;
		else if (argv[1][i] == ')')
		{
			if (rm_s > 0)
				rm_s--;
			else
				rm_e++;
		}
		i++;
	}

	dfs(argv[1], 0, rm_s, rm_e);
	return (0);
}
