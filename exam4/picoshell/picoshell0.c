
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	close_all(int prev_fd, int *fd, int has_next)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (has_next)
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	parent_process(int *prev_fd, int *fd, int has_next)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
}

void	child_process(int prev_fd, int *fd, char **cmd, int has_next)
{
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
		{
			close_all(prev_fd, fd, has_next);
			exit(1);
		}
		close(prev_fd);
	}
	if (has_next)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			close_all(prev_fd, fd, has_next);
			exit(1);
		}
		close(fd[0]);
		close(fd[1]);
	}
	execvp(cmd[0], cmd);
	exit(1);
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
	if (!cmd || !cmd[0])
		return (1);
	while (cmd[i])
	{
		has_next = (cmd[i + 1] != NULL);
		if (has_next)
		{
			if (pipe(fd) == -1)
				return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			close_all(prev_fd, fd, has_next);
			return (1);
		}
		if (pid == 0)
			child_process(prev_fd, fd, cmd[i], has_next);
		else
			parent_process(&prev_fd, fd, has_next);
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

// int	main(void)
// {
// 	char	*cmd1[] = {"ls", "-a", NULL};
// 	char	*cmd2[] = {"cat", NULL};
// 	char	**cmd[] = {cmd1, cmd2, NULL};

// 	picoshell(cmd);
// }

int	main(void)
{
	char	*cmd1[] = {"echo", "squalala", NULL};
	char	*cmd2[] = {"cat", NULL};
	char	*cmd3[] = {"sed", "s/a/b/g", NULL};
	char	**str[] = {cmd1, cmd2, cmd3, NULL};

	picoshell(str);
}
