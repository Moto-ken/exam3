
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void	child_process(int prev_fd, int *fd, int has_next, char **argv)
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
	execvp(argv[0], argv);
	exit(1);
}

void	parent_process(int *prev_fd, int *fd, int has_next)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (has_next)
	{
		*prev_fd = fd[0];
		close(fd[1]);
	}
}

int	picoshell(char **argv[])
{
	int		fd[2];
	pid_t	pid;
	int		i;
	int		has_next;
	int		prev_fd;

	has_next = 0;
	prev_fd = -1;
	i = 0;
	if (!argv || !argv[0])
		return (1);
	while (argv[i])
	{
		has_next = argv[i + 1] != NULL;
		if (has_next)
		{
			if (pipe(fd) < 0)
				return (1);
		}
		pid = fork();
		if (pid < 0)
		{
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (pid == 0)
			child_process(prev_fd, fd, has_next, argv[i]);
		else
			parent_process(&prev_fd, fd, has_next);
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	main(void)
{
	char	*cmd1[] = {"/bin/ls", NULL};
	char	*cmd2[] = {"cat", NULL};
	char	**cmds[] = {cmd1, cmd2, NULL};

	picoshell(cmds);
	return (0);
}
