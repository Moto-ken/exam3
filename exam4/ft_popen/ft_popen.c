
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void	child_process(int *fd, const char *file, char *const argv[], char type)
{
	if (type == 'r')
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit(1);
	}
	else if (type == 'w')
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			exit(1);
	}
	else
		exit(1);
	close(fd[0]);
	close(fd[1]);
	execvp(file, argv);
	exit(1);
}

int	parent_process(int *fd, char type)
{
	// usleep(1000);
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	else
		return (-1);
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	pid_t	pid;
	int		fd[2];

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
	if (pid == 0)
		child_process(fd, file, argv, type);
	return (parent_process(fd, type));
}

// int	main(void)
// {
// 	int		fd;
// 	ssize_t	n;
// 	char	buf[1000];

// 	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
// 	while ((n = read(fd, buf, sizeof(buf) - 1)) > 0)
// 	{
// 		buf[n] = '\0';
// 		printf("%s", buf);
// 	}
// 	close(fd);
// 	return (0);
// }

int	main(void)
{
	int		fd;
	char	*data;

	data = "8\n3\n7\n2";
	fd = ft_popen("sort", (char *const[]){"sort", NULL}, 'w');
	write(fd, data, strlen(data));
	close(fd);
	return (0);
}
