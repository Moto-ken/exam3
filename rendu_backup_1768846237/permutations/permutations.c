
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

void back_track(char *str, size_t index, size_t max, char *re, int *used)
{
	if (index == max)
	{
		re[index] = '\0';
		puts(re);
		return ;
	}
	size_t i = 0;
	while (i < max)
	{
		if (!used[i])
		{
			used[i] = 1;
			re[index] = str[i];
			back_track(str, index + 1, max, re, used);
			used[i] = 0;
		}
		i++;
	}
	return ;
}

void sort_str(char *str)
{
	int i = 0;
	char tmp;

	while (str[i])
	{
		int j = i + 1;
		while (str[j])
		{
			if (str[i] > str[j])
			{
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	char *str = argv[1];
	size_t len = ft_strlen(str);
	char *result = malloc(len + 1);
	if (!result)
		return (1);
	int *used = calloc(len, sizeof(int));
	if (!used)
	{
		free(result);
		return (1);
	}
	sort_str(str);
	back_track(str, 0, len, result, used);
	free(result);
	free(used);
	return (0);
}


// #include <stdio.h>

// void swap(char *a, char *b)
// {
//     char tmp = *a;
//     *a = *b;
//     *b = tmp;
// }

// void permute(char *s, int pos)
// {
// 	if (!s[pos])
// 	{
// 		puts(s);
// 		return;
// 	}
// 	int i = pos;
// 	while (s[i])
// 	{
// 		swap(&s[pos], &s[i]);
// 		permute(s, pos + 1);
// 		swap(&s[pos], &s[i]);
// 		i++;
// 	}
// }

// int main(int argc, char **argv)
// {
//     if (argc != 2)
//         return 1;
// 	sort_str(argv[1]);
//     permute(argv[1], 0);
//     return 0;
// }
