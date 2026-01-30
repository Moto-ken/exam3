
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *make_list(size_t count, char **argv)
{
	int *list = malloc(sizeof(int) * count);
	if (!list)
		return (NULL);

	size_t i = 0;
	size_t j = 2;
	while (count > i)
	{
		list[i] = atoi(argv[j]);
		i++;
		j++;
	}
	return (list);
}


void dfs(int index, int max, int depth, int *list, int *current, int sum)
{
	if (sum == 0)
	{
		size_t i = 0;
		while (depth > i)
		{
			if (i)
				printf(" ");
			printf("%d", current[i]);
			i++;
		}
		printf("\n");
		return;
	}
	if (index >= max || sum < 0)
		return;
	current[depth] = list[index];
	dfs(index + 1, max, depth + 1, list, current, sum - list[index]);
	dfs(index + 1, max, depth, list, current, sum);
}

int main(int argc, char **argv)
{
	if (argc < 3)
		return (1);
	
	int sum = atoi(argv[1]);
	size_t count = argc - 2;
	int *list = make_list(count, argv);
	int *current = malloc(sizeof(int) * count);
	if (!current)
	{
		free(list);
		return (1);
	}
	
	// size_t i = 0;
	// while (i < count)
	// {
	// 	printf("%d ", list[i]);
	// 	i++;
	// }
	// printf("\n");
	dfs(0, count, 0, list, current, sum);

	free(list);
	free(current);
	return (0);
}
