
#include "n_queens.h"

int is_safe(char **map, int n, int row, int col)
{
	int i, j;

	i = 0;
	while (i < row)
	{
		if (map[i][col] == 'Q')
			return (0);
		i++;
	}
	i = row - 1;
	j = col - 1;
	while (i >= 0 && j >= 0)
	{
		if (map[i][j] == 'Q')
			return (0);
		i--;
		j--;
	}
	i = row - 1;
	j = col + 1;
	while (i >= 0 && j < n)
	{
		if (map[i][j] == 'Q')
			return (0);
		i--;
		j++;
	}
	return (1);
}

void print_map(char **map, int n)
{
	int i = 0;
	while (i < n)
	{
		int j = 0;
		while (j < n)
		{
			if (map[i][j] == 'Q')
			{
				fprintf(stdout, "%d", j);
				if (!(i == n - 1))
					fprintf(stdout, " ");
				break;
			}
			j++;
		}
		i++;
	}
	fprintf(stdout, "\n");
}

void back_track(int n, char **map, int row)
{
	int col;

	if (row == n)
	{
		print_map(map, n);
		return;
	}
	col = 0;
	while (col < n)
	{
		if (is_safe(map, n, row, col))
		{
			map[row][col] = 'Q';
			back_track(n, map, row + 1);
			map[row][col] = '0';
		}
		col++;
	}
}

char **make_map(int n)
{
	char **result;
	int i = 0;

	result = calloc(n, sizeof(char *));
	if (!result)
		return (NULL);
	while (i < n)
	{
		result[i] = calloc(n + 1, sizeof(char));
		if (!result[i])
		{
			while (0 <= i)
			{
				free(result[i]);
				i--;
			}
			free(result);
			return (NULL);
		}
		i++;
	}
	int j = 0;
	while (j < n)
	{
		int k = 0;
		while (k < n)
		{
			result[j][k] = '0';
			k++;
		}
		j++;
	}
	return (result);
}

int  main(int argc, char *argv[])
{
	int n;
	char **map;

	if (argc != 2)
		return (1);
	if (!(n = atoi(argv[1])))
		return(1);
	map = make_map(n);
	if (!map)
		return (1);
	back_track(n, map, 0);
	int i = 0;
	while (n > i)
	{
		free(map[i]);
		i++;
	}
	free(map);
	return (0);
}
