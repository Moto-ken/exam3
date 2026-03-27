
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int	picoshell(char **cmd[])
{
	int		fd[2];
	int		prev_fd;
	pid_t	pid;
	int		i;

	i = 0;
	prev_fd = -1;
	while (cmd[i] != NULL)
	{
		if (cmd[i + 1] != NULL)
		{
			if (pipe(fd) < 0)
				exit(1);
		}
		pid = fork();
		if (pid < 0)
			return (0);
		else if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, 0) == -1)
					exit(1);
				close(prev_fd);
			}
			if (cmd[i + 1] != NULL)
			{
				close(fd[0]);
				if (dup2(fd[1], 1) == -1)
					exit(1);
				close(fd[1]);
			}
			execvp(cmd[i][0], cmd[i]);
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd[i + 1] != NULL)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	main(void)
{
	char	*cmd1[] = {"ls", "-a", NULL};
	char	*cmd2[] = {"wc", NULL};
	char	**str[] = {cmd1, cmd2, NULL};
	int		re;

	re = picoshell(str);
	return (0);
}
