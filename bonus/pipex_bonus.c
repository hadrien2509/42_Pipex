/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgeissle <hgeissle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 11:41:30 by hgeissle          #+#    #+#             */
/*   Updated: 2023/03/23 18:36:45 by hgeissle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < 2 * pipex->pipelen)
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

void	show_err(char *err)
{
	write(2, err, ft_strlen(err));
	exit (1);
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
	if (pipex->pipeline)
		free(pipex->pipeline);
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
	}
	close_pipes(&pipex);
	execve(pipex.tab[0], pipex.tab, envp);
}

void	select_process(t_pipex pipex, char **av, char **envp, int ac)
{
	if (pipex.i == 0)
		child_process(pipex, av, envp);
	if (pipex.i == ac - 4 - pipex.here_doc)
		parent_process(pipex, av, envp);
	else
		pipe_process(pipex, av, envp);
}

void	here_doc(t_pipex *pipex, char **av)
{
	int		len;
	char	*line;
	char	*strs[2];

	pipex->infile = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->infile == -1)
		show_err(ERR_HEREDOC);
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		line = get_next_line(0);
		if (!line)
			exit (1);
		len = ft_strlen(line) - 1;
		if (len == ft_strlen(av[2]) && ft_strncmp(line, av[2], len) == 0)
			break ;
		write(pipex->infile, line, len + 1);
		free(line);
	}
	free(line);
	close(pipex->infile);
}

void	ft_setfd(t_pipex *pipex, int ac, char **av)
{
	pipex->here_doc = 0;
	pipex->outfile = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		show_err(ERR_OUTFILE);
	if (ft_strncmp("here_doc", av[1], ft_strlen(av[1])) == 0)
	{
		pipex->here_doc = 1;
		if (ac < 6)
			show_err(ERR_INPUT);
		here_doc(pipex, av);
		pipex->infile = open(".heredoc_tmp", O_RDONLY);
		if (pipex->infile < 0)
		{
			unlink(".heredoc_tmp");
			show_err(ERR_HEREDOC);
		}
	}
	else
	{
		pipex->infile = open(av[1], O_RDONLY);
		if (pipex->infile == -1)
			show_err(ERR_INFILE);
	}
}

void	ft_setstruct(t_pipex *pipex, int ac, char **envp)
{
	pipex->pipeline = (int *)malloc(sizeof(int) * (2 * pipex->pipelen));
	if (!pipex->pipeline)
		show_err(ERR_PIPE);
	pipex->paths = ft_getallpaths(envp);
	if (!pipex->paths)
	{
		ft_free_pipex(pipex);
		exit (1);
	}
	pipex->tab = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;

	if (ac < 5)
		show_err(ERR_INPUT);
	ft_setfd(&pipex, ac, av);
	ft_setstruct(&pipex, ac, envp);
	pipex.i = 0;
	pipex.pipelen = ac - 4 - pipex.here_doc;
	while (pipex.i <= ac - 4 - pipex.here_doc)
	{
		if (pipex.i < pipex.pipelen && pipe(pipex.pipeline + pipex.i * 2) == -1)
			show_err(ERR_PIPE);
		pipex.child = fork();
		if (!pipex.child)
			select_process(pipex, av, envp, ac);
		pipex.i++;
	}
	if (pipex.here_doc)
		unlink(".heredoc_tmp");
	close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	ft_free_pipex(&pipex);
	return (0);
}
