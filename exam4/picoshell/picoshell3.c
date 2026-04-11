
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	child_process(int *fd, int prev_fd, int has_next, char **cmd)
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

void	parent_process(int *fd, int *prev_fd, int has_next)
{
	if (*prev_fd != -1)
	{
		close(*prev_fd);
	}
	if (has_next)
	{
		*prev_fd = fd[0];
		close(fd[1]);
	}
}

int	picoshell(char **cmds[])
{
	int		fd[2];
	pid_t	pid;
	int		i;
	int		prev_fd;
	int		has_next;

	if (!cmds)
		return (0);
	i = 0;
	prev_fd = -1;
	has_next = 0;
	while (cmds[i])
	{
		has_next = cmds[i + 1] != NULL;
		if (has_next)
		{
			if (pipe(fd) < 0)
				return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			if (prev_fd != -1)
				close(prev_fd);
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (pid == 0)
			child_process(fd, prev_fd, has_next, cmds[i]);
		else
			parent_process(fd, &prev_fd, has_next);
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	main(void)
{
	char	*cmd1[] = {"ls", NULL};
	char	*cmd2[] = {"grep", "picoshell", NULL};
	char	**cmd[] = {cmd1, cmd2, NULL};

	picoshell(cmd);
	return (0);
}
