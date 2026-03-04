
#include <unistd.h>
#include <stdio.h>

int is_safe(char *str)
{
	int i = 0;
	int s_count = 0;

	while (str[i])
	{
		if (str[i] == '(')
			s_count++;
		else if (str[i] == ')')
			s_count--;
		if (s_count < 0)
			return (0);
		i++;
	}
	return (s_count == 0);
}

void dfs(char *str, int s_count, int e_count, int index, int max)
{
	if (s_count == 0 && e_count == 0 && is_safe(str))
	{
		puts(str);
		return;
	}
	if (max == index)
		return;
	if (s_count > 0 && str[index] == '(')
	{
		str[index] = ' ';
		dfs(str, s_count - 1, e_count, index + 1, max);
		str[index] = '(';
	}
	if (e_count > 0 && str[index] == ')')
	{
		str[index] = ' ';
		dfs(str, s_count, e_count - 1, index + 1, max);
		str[index] = ')';
	}
	dfs(str, s_count, e_count, index + 1, max);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return (1);
	int i = 0;
	while (argv[1][i])
	{
		if  (!(argv[1][i] == '(' || argv[1][i] == ')' || argv[1][i] == ' '))
			return (1);
		i++;
	}
	int s_count = 0;
	int e_count = 0;
	i = 0;
	while (argv[1][i])
	{
		if (argv[1][i] == '(')
			s_count++;
		else if (argv[1][i] == ')')
		{
			if (s_count > 0)
				s_count--;
			else
				e_count++;
		}
		i++;
	}
	// if (is_safe(argv[1]))
	// 	printf("%s | %d | %d \n", argv[1], s_count, e_count);
	dfs(argv[1], s_count, e_count, 0, i);
	return (0);
}
