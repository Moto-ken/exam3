
#include "n_queens.h"

char **make_map(int n)
{
	char **res;

	res = malloc(sizeof(char *) * n);
	if (!res)
		return (NULL);
	int i = 0;
	while (i < n)
	{
		res[i] = malloc(n + 1);
		if (!res)
		{
			free(res);
			return (NULL);
		}
		int init_i = 0;
		while (init_i < n)
		{
			res[i][init_i] = '0';
			init_i++;
		}
		res[i][n] = '\0';
		i++;
	}
	return (res);
}

int is_safe(char **map, int y, int x, int n)
{
	// if (y == 0)
	// 	return (1);
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
		x_i--;
		y_i--;
	}
	y_i = y - 1;
	x_i = x + 1;
	while (0 <= y_i && x_i < n)
	{
		if (map[y_i][x_i] == 'Q')
			return (0);
		x_i++;
		y_i--;
	}
	return (1);
}

void print(char **map, int n)
{
	int y = 0;
	while (n > y)
	{
		int x = 0;
		while (n > x)
		{
			if (map[y][x] == 'Q')
			{
				fprintf(stdout, "%d", x);
				if (!(y == n - 1))
					fprintf(stdout, " ");
			}
			x++;
		}
		y++;
	}
	fprintf(stdout, "\n");
}

void back_track(char **map, int y, int n)
{
	int x = 0;

	if (y == n)
	{
		print(map, n);
		return;
	}

	while (map[y][x])
	{
		if (is_safe(map, y, x, n))
		{
			map[y][x] = 'Q'; 
			back_track(map, y + 1, n);
			map[y][x] = '0';
		}
		// if (x == n)
		// 	break;
		x++;
	}
	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		return (1);
	
	int n = atoi(argv[1]);
	if (n < 0)
		return (1);
	// printf("%d\n", n);
	char **map;

	map = make_map(n);
	if (!map)
		return (1);
	// print(map);
	back_track(map, 0, n);
	int i = 0;
	while (i < n)
	{
		free(map[i]);
		i++;
	}
	free(map);
	return (0);
}
