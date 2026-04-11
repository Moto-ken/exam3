
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void	child_process(const char *file, char *const argv[], char type, int *fd)
{
	if (type == 'r')
	{
		if (dup2(fd[1], STDOUT_FILENO) < 0)
		{
			close(fd[0]);
			close(fd[1]);
			exit(1);
		}
	}
	else if (type == 'w')
	{
		if (dup2(fd[0], STDIN_FILENO) < 0)
		{
			close(fd[0]);
			close(fd[1]);
			exit(1);
		}
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	// sleep(1000);
	execvp(file, argv);
	exit(1);
}

int	parent_process(int type, int *fd)
{
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else if (type == 'w')
	{
		// printf("%d\n", fd[1]);
		close(fd[0]);
		return (fd[1]);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	return (-1);
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	else if (pid == 0)
		child_process(file, argv, type, fd);
	// printf("hear");
	return (parent_process(type, fd));
}

// int	main(void)
// {
// 	int		fd;
// 	char	buf[1000];
// 	ssize_t	n;

// 	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
// 	while (n = read(fd, buf, sizeof(buf) - 1))
// 	{
// 		buf[n] = '\0';
// 		printf("%s", buf);
// 	}
// 	close(fd);
// 	return (0);
// }

int	main(void)
{
	char	*data;
	int		fd;

	data = "6\n9\n2\n7\n";
	fd = ft_popen("sort", (char *const[]){"sort", NULL}, 'w');
	write(fd, data, strlen(data));
	close(fd);
	return (0);
}
