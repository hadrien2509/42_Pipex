/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/22 18:57:35 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		close(pipex->pipeline[i]);
		i++;
	}
}

// static int	creat_pipes(t_pipex *pipex)
// {
// 	int	i;

// 	i = 0;
// 	while (i < 2)
// 	{
// 		if (pipe(pipex->pipeline + 2 * i) < 0)
// 			return (show_err(ERR_PIPE));
// 		i++;
// 	}
// 	return (0);
// }

int	show_err(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

void	ft_free_pipex(t_pipex *pipex)
{
	int		i;

	if (pipex->tab)
	{
		i = 0;
		while (pipex->tab[i])
		{
			free(pipex->tab[i]);
			i++;
		}
		free(pipex->tab);
	}
	if (pipex->paths)
	{
		i = 0;
		while (pipex->paths[i])
		{
			free(pipex->paths[i]);
			i++;
		}
		free(pipex->paths);
	}
}

void	parent_process(t_pipex pipex, char **av, char **envp)
{
	char	**tab;

	dup2(pipex.pipeline[pipex.i * 2 - 2], 0);
	close(pipex.pipeline[pipex.i * 2 - 1]);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	ft_pathname(av[2 + pipex.i + pipex.here_doc], &pipex, envp);
	if (!pipex.tab)
	{
		ft_free_pipex(&pipex);
		show_err(ERR_CMD);
		exit(1);
	}
	close_pipes(&pipex);
	execve(pipex.tab[0], pipex.tab, envp);
}

void	select_process(t_pipex pipex, char **av, char **envp, int ac)
{
	if (pipex.i == 0)
		child_process(pipex, av, envp);
	if (pipex.i == ac - 4)
		parent_process(pipex, av, envp);
	else
		pipe_process(pipex, av, envp);
}

int	here_doc(t_pipex *pipex, char **av)
{
	int		len;
	char	*line;

	pipex->here_doc = 1;
	pipex->infile = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC);
	if (pipex->infile == -1)
		return (show_err(ERR_INFILE));
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (!line)
			return (1);
		len = ft_strlen(line);
		if (ft_strncmp(line, av[2], len - 1) == 0)
			break ;
		write(pipex->infile, line, len);
		free(line);
	}
	free(line);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	pipex.here_doc = 0;
	if (ac < 5)
		return (show_err(ERR_INPUT));
	if (ft_strncmp("here_doc", av[1], 8) == 0)
		here_doc(&pipex, av);
	else
	{
		pipex.infile = open(av[1], O_RDONLY);
		if (pipex.infile == -1)
			return (show_err(ERR_INFILE));
	}
	pipex.outfile = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex.outfile == -1)
		return (show_err(ERR_OUTFILE));
	pipex.pipeline = (int *)malloc(sizeof(int) * (2 * (ac - 3)));
	if (!pipex.pipeline)
		return (show_err(ERR_PIPE));
	pipex.paths = ft_getallpaths(envp);
	if (!pipex.paths)
	{
		ft_free_pipex(&pipex);
		return (0);
	}
	pipex.tab = 0;
	pipex.i = 0;
	while (pipex.i <= ac - 4)
	{
		if (pipex.i < ac - 4 && pipe(pipex.pipeline + pipex.i * 2) == -1)
			return (show_err(ERR_PIPE));
		pipex.child = fork();
		if (!pipex.child)
			select_process(pipex, av, envp, ac);
		pipex.i++;
	}
	if (pipex.here_doc)
		unlink(".heredoc_tmp");
	waitpid(-1, NULL, 0);
	close_pipes(&pipex);
	ft_free_pipex(&pipex);
	return (0);
}
