#include <string.h>  
#include <stdlib.h>
#include <unistd.h>

typedef int t_bool;

int	ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

void	print(char *s)
{
	write(2, s, ft_strlen(s));
}

void	fatal(void)
{
	print("error: fatal\n");
	exit(0);
}

int	ft_cd(char **argv)
{
	if (argv[1] == NULL)
		print("error: cd: bad arguments\n");
	else if (chdir(argv[1]))
	{
		print("error: cd: cannot change directory to ");
		print(argv[1]);
		print("\n");
	}
	return (0);
}

int	parent(pid_t pid, int fd[][2], int p, t_bool pipe_prev, t_bool pipe_next)
{
	int stat = 0;

	waitpid(pid, &stat, 0);
	if (pipe_prev || pipe_next)
	{
		if (pipe_prev)
			close(fd[p - 1][0]);
		if (!pipe_next)
			close(fd[p][0]);
		close(fd[p][1]);
	}
	return (0);
}

int	child(char **argv, char **env, int fd[][2], int p, t_bool pipe_prev, t_bool pipe_next)
{
	int ret = 0;

	if (pipe_prev)
	{
		if (dup2(fd[p - 1][0], STDIN_FILENO) < 0)
			fatal();
	}
	if (pipe_next)
	{
		if (dup2(fd[p][1], STDOUT_FILENO) < 0)
			fatal();
	}
	ret = execve(argv[0], &argv[0], env);
	if (ret < 0)
	{
		print("error: cannot execute ");
		print(argv[0]);
		print("\n");
	}
	exit(ret);
}

int	ft_execve(char **argv, char **env, int fd[][2], int p, t_bool pipe_prev, t_bool pipe_next)
{
	if (pipe_prev || pipe_next)
	{
		if (pipe(fd[p]))
			fatal();
	}
	pid_t pid = fork();
	if (pid < 0)
		fatal();
	if (pid)
		parent(pid, fd, p, pipe_prev, pipe_next);
	else
		child(argv, env, fd, p, pipe_prev, pipe_next);
	return (0);
}

int cmd(char **argv, char **env, int fd[][2], int p, t_bool pipe_prev, t_bool pipe_next)
{
	if (!strncmp(argv[0], "cd", 3))
		ft_cd(argv);
	else
		ft_execve(argv, env, fd, p, pipe_prev, pipe_next);
	return (0);
}

int split_pipes(int argc, char **argv, char **env)
{
	int pipe_count = 0;

	for (int i = 0; i < argc; i++)
	{
		if (!strncmp(argv[i], "|", 2))
		{
			argv[i] = NULL;
			pipe_count++;
		}
	}
	int fd[pipe_count + 1][2];
	int p = 0;

	int i = 0;
	while (i < argc)
	{
		int j = i;
		while (argv[j])
			j++;
		cmd(&argv[i], env, fd, p, i, argc - j);
		p++;
		i = j + 1;
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	for (int i = 1; i < argc; i++)
	{
		if (!strncmp(argv[i], ";", 2))
			argv[i] = NULL;
	}
	int i = 1;
	while (i < argc)
	{
		int j = i;
		while (argv[j])
			j++;
		split_pipes(j - i, &argv[i], env);
		i = j + 1;
	}
	return (0);
}
