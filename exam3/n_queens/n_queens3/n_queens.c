
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void free_map(char **map, int n)
{
	int i = 0;
	while (i < n)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

char **make_map(int n)
{
	char **map = malloc(sizeof(char *) * n);
	if (!map)
		return (NULL);
	size_t i = 0;
	while (i < n)
	{
		map[i] = malloc(n + 1);
		if (!map[i])
		{
			free_map(map, n);
			return (NULL);
		}
		size_t j = 0;
		while (j < n)
		{
			map[i][j] = '0';
			j++;
		}
		map[i][j] = '\0';
		i++;
	}
	return (map);
}

int issafe(char **map, int y, int x, int n)
{
	int i = 0;
	while (i < y)
	{
		if (map[i][x] == 'Q')
			return (0);
		i++;
	}
	int y_i = y - 1;
	int x_i = x - 1;
	while (0 <= y_i && 0 <= x_i)
	{
		if (map[y_i][x_i] == 'Q')
			return (0);
		y_i--;
		x_i--;
	}
	y_i = y - 1;
	x_i = x + 1;
	while (0 <= y_i && x_i <= n)
	{
		if (map[y_i][x_i] == 'Q')
			return (0);
		y_i--;
		x_i++;
	}
	return (1);
}

void print_map(char **map, int n)
{
	int y = 0;
	while (y < n)
	{
		int x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'Q')
			{
				printf("%d", x);
				if (y != n - 1)
					printf(" ");
			}
			x++;
		}
		y++;
	}
	printf("\n");
}

void dfs(int y, char **map, int n)
{
	int x = 0;
	if (n == y)
	{
		print_map(map, n);
		return;
	}
	while (x < n)
	{
		if (issafe(map, y, x, n))
		{
			map[y][x] = 'Q';
			dfs(y + 1, map, n);
			map[y][x] = '0';
		}
		x++;
	}
	return;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	
	int n = atoi(argv[1]);
	if (n <= 0)
		return (1);
	char **map = make_map(n);
	if (!map)
		return (1);
	
	dfs(0, map, n);

	free_map(map, n);
	return (0);
}
