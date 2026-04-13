
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	child_process(char *cmd[], int prev_fd, int has_next, int *fd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	execvp(cmd[0], cmd);
	exit(1);
}

void	parent_process(int *prev_fd, int has_next, int *fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
}

int	picoshell(char **cmd[])
{
	int		fd[2];
	pid_t	pid;
	int		i;
	int		prev_fd;
	int		has_next;

	i = 0;
	prev_fd = -1;
	has_next = 0;
	if (!cmd)
		return (1);
	while (cmd[i])
	{
		has_next = cmd[i + 1] != NULL;
		if (has_next)
		{
			if (pipe(fd) < 0)
			{
				if (prev_fd != -1)
					close(prev_fd);
				return (1);
			}
		}
		pid = fork();
		if (pid < 0)
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (has_next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			return (1);
		}
		if (pid == 0)
			child_process(cmd[i], prev_fd, has_next, fd);
		else
			parent_process(&prev_fd, has_next, fd);
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	main(void)
{
	char	*str[] = {"ls", NULL};
	char	*str1[] = {"grep", "picoshell", NULL};
	char	**cmd[] = {str, str1, NULL};

	picoshell(cmd);
	return (0);
}
