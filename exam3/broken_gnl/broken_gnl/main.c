
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
	int		fd;
	char	*res;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("open faild");
		return(1);
	}
	while ((res = get_next_line(fd)) != NULL)
	{
		printf("%s", res);
		free(res);
	}
	close(fd);
	return (0);
}
