
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	child_process(char *cmd[], int *fd, int prev_fd, int has_next)
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
		close(*prev_fd);
	if (has_next)
	{
		*prev_fd = fd[0];
		close(fd[1]);
	}
}

int	picoshell(char **cmd[])
{
	int		fd[2];
	pid_t	pid;
	int		prev_fd;
	int		i;
	int		has_next;

	if (!cmd || !cmd[0])
		return (1);
	prev_fd = -1;
	i = 0;
	has_next = 0;
	while (cmd[i])
	{
		has_next = cmd[i + 1] != NULL;
		if (has_next)
			pipe(fd);
		pid = fork();
		if (pid < 0)
		{
			if (prev_fd != -1)
				close(prev_fd);
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		else if (pid == 0)
			child_process(cmd[i], fd, prev_fd, has_next);
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
	char	*cmd[] = {"ls", "-a", NULL};
	char	*cmd2[] = {"grep", "picoshell", NULL};
	char	**str[] = {cmd, cmd2, NULL};

	picoshell(str);
}
