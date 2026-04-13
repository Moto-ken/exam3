
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct s_cmd
{
	char			**argv;
	int				fd[2];
	struct s_cmd	*next;
}					t_cmd;

void	free_struct(t_cmd *head)
{
	t_cmd	*cur;
	t_cmd	*tmp;

	cur = head;
	while (cur)
	{
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

t_cmd	*init_struct(char ***cmd)
{
	t_cmd	*head;
	t_cmd	*prev;
	int		i;
	t_cmd	*cur;

	head = NULL;
	prev = NULL;
	i = 0;
	while (cmd[i])
	{
		cur = malloc(sizeof(t_cmd));
		if (!cur)
		{
			free_struct(head);
			return (NULL);
		}
		cur->argv = cmd[i];
		cur->next = NULL;
		cur->fd[0] = -1;
		cur->fd[1] = -1;
		if (prev)
			prev->next = cur;
		else
			head = cur;
		prev = cur;
		i++;
	}
	return (head);
}

void	child_process(t_cmd *cur, int in_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cur->next != NULL)
	{
		dup2(cur->fd[1], STDOUT_FILENO);
		close(cur->fd[0]);
		close(cur->fd[1]);
	}
	execvp(cur->argv[0], cur->argv);
	exit(1);
}

void	parent_process(t_cmd **cur_ptr, int *in_fd)
{
	t_cmd	*cur;

	cur = *cur_ptr;
	if (*in_fd != 0)
		close(*in_fd);
	if (cur->next != NULL)
		close(cur->fd[1]);
	if (cur->next != NULL)
		*in_fd = cur->fd[0];
	else
		*in_fd = 0;
	*cur_ptr = cur->next;
}

int	picoshell(char **cmds[])
{
	t_cmd	*head;
	t_cmd	*cur;
	pid_t	pid;
	int		in_fd;

	in_fd = 0;
	if ((head = init_struct(cmds)) == NULL)
		return (1);
	cur = head;
	while (cur)
	{
		if (cur->next != NULL)
		{
			if (pipe(cur->fd) < 0)
			{
				free_struct(head);
				return (1);
			}
		}
		pid = fork();
		if (pid < 0)
		{
			free_struct(head);
			return (1);
		}
		if (pid == 0)
			child_process(cur, in_fd);
		else
			parent_process(&cur, &in_fd);
	}
	while (wait(NULL) > 0)
		;
	free_struct(head);
	return (0);
}

int	main(void)
{
	char	*cmd1[] = {"echo", "squalala", NULL};
	char	*cmd2[] = {"cat", NULL};
	char	*cmd3[] = {"sed", "s/a/b/g", NULL};
	char	**test2[] = {cmd1, cmd2, cmd3, NULL};

	// char	*cmd1[] = {"/bin/ls", NULL};
	// char	*cmd2[] = {"/usr/bin/grep", "picoshell", NULL};
	// char	**test1[] = {cmd1, cmd2, NULL};
	// picoshell(test1);
	picoshell(test2);
	return (0);
}
