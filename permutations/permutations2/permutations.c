
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

size_t ft_strlen(char *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	return (i);
}

void dfs(char *str, int count, int *check, char *result, int index)
{
	if (index == count)
	{
		result[count + 1] = '\0';
		puts(result);
		return;
	}
	int i = 0;
	while (i < count)
	{
		if (!check[i])
		{
			result[index] = str[i];
			check[i] = 1;
			dfs(str, count, check, result, index + 1);
			check[i] = 0;
		}

		i++;
	}

}

void bable_sort(char *str)
{
	int i = 0;
	char *tmp;
	while (str[i])
	{
		int j = 0;
		while (str[j])
		{
			if (str[i] < str[j])
			{
				*tmp = str[i];
				str[i] = str[j];
				str[j] = *tmp;
			}
			j++;
		}
		i++;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2 || argv[1][0] == '\0')
		return (1);

	size_t count = ft_strlen(argv[1]);
	char *str = argv[1];
	int *check = malloc(sizeof(int) * count);
	if (!check)
		return (1);
	size_t i = 0;
	while (i < count)
	{
		check[i] = '0';
		i++;
	}
	char *result = malloc(count + 1);
	if (!result)
	{
		free(check);
		return (1);
	}
	int j = 0;
	while (j < count)
	{
		check[j] = 0;
		j++;
	}

	bable_sort(str);
	printf("check cort:%s\n", str);
	// int k = 0;
	// while (k < count)
	// {
	// 	printf("%d ", result[k]);
	// 	k++;
	// }
	// printf("\n");
	dfs(str, count, check, result, 0);

	free(check);
	free(result);
	return (0);
}
